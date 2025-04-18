#!/usr/bin/env bash

set -uo pipefail

declare -i FAILED=0
declare -i SUCCEDED=0
declare -i TODO=0
declare -i RAN=0
NAME=""
EXTRAENV=""
MINISHELL=$PWD/minishell

report_header()
{
	echo "++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo $NAME
	echo "++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "++++ test input:"
}

failed_report()
{
	NAME="failed: $NAME"
	report_header
	cat /tmp/input.minishell
	echo "++++ Got output:"
	cat /tmp/got.minishell
	echo "++++ But expected:"
	cat /tmp/expected.minishell
	echo "++++ Diff:"
	echo "(red is got, green is expected)"
	diff --color=always /tmp/got.minishell /tmp/expected.minishell
	echo "++++ Run this to debug:"
	cat < /tmp/input.minishell > /tmp/input.failed.$FAILED.minishell
	echo "cat /tmp/input.failed.$FAILED.minishell | ./minishell"
}

todo_report()
{
	NAME="todo: $NAME"
	report_header
	cat /tmp/input.minishell
	echo "++++ Got output:"
	cat /tmp/got.minishell
	echo "++++ But test was marked as todo."
	echo "++++ Run this to debug:"
	cat < /tmp/input.minishell > /tmp/input.todo.$TODO.minishell
	echo "cat /tmp/input.todo.$TODO.minishell | ./minishell"
}

failed()
{
	echo -n "F"
	FAILED+=1
	failed_report >> report.txt
}

succeded()
{
	echo -n "."
	SUCCEDED+=1
}

assert()
{
	diff -q /tmp/got.minishell /tmp/expected.minishell > /dev/null && succeded || failed
}

when_run()
{
	cat > /tmp/input.minishell
	rm -rf /tmp/dir.minishell
	mkdir -p /tmp/dir.minishell
	pushd /tmp/dir.minishell > /dev/null
	env $EXTRAENV $MINISHELL &> /tmp/got.minishell < /tmp/input.minishell
	popd > /dev/null
	EXTRAENV=""
	RAN+=1
	NAME=$1
}

expecting()
{
	cat > /tmp/expected.minishell
	assert
}

todo()
{
	echo -n "t"
	TODO+=1
	todo_report >> report.txt
}

setup()
{
	rm -f report.txt
}

finalize()
{
	echo
	echo "$RAN tests ran. $SUCCEDED succeded, $FAILED failed, $TODO todo."
	if [ $SUCCEDED -eq $RAN ];
	then
		echo "All integration tests passed!"
	else
		echo "cat report.txt"
		echo "to see details of failed/todo tests"
	fi
	exit $FAILED
}

setup

when_run <<EOF "empty inputs returns nothing"

EOF
expecting <<EOF
EOF

when_run <<EOF "only spaces returns nothing"

EOF
expecting <<EOF
EOF

when_run <<EOF "only tabs returns nothing"

EOF
expecting <<EOF
EOF

when_run <<EOF "/ is a directory"
/
echo \$?
EOF
expecting <<EOF
minishell: /: Is a directory
126
EOF

when_run <<EOF "// is a directory"
//
echo \$?
EOF
expecting <<EOF
minishell: //: Is a directory
126
EOF

when_run <<EOF "/. is a directory"
/.
echo \$?
EOF
expecting <<EOF
minishell: /.: Is a directory
126
EOF

when_run <<EOF "/./../../../../.. is a directory"
/./../../../../..
echo \$?
EOF
expecting <<EOF
minishell: /./../../../../..: Is a directory
126
EOF

when_run <<EOF "//////// is a directory"
////////
echo \$?
EOF
expecting <<EOF
minishell: ////////: Is a directory
126
EOF

when_run <<EOF "simple commands are run"
echo no files:
ls -a
touch t
echo single file:
ls -a
EOF
expecting <<EOF
no files:
.
..
single file:
.
..
t
EOF

when_run <<EOF "unknown commands"
qwertyuiop
echo \$?
poiuytrewq
echo \$?
EOF
expecting <<EOF
minishell: qwertyuiop: command not found
127
minishell: poiuytrewq: command not found
127
EOF

when_run <<EOF "pwd works"
pwd
pwd hello
pwd hello there
pwd -O
pwd -O hello
pwd there -O hello
EOF
expecting <<EOF
/tmp/dir.minishell
/tmp/dir.minishell
/tmp/dir.minishell
/tmp/dir.minishell
/tmp/dir.minishell
/tmp/dir.minishell
EOF

when_run <<EOF "pwd works in any directory, can cd to absolute paths"
mkdir test
cd /tmp/dir.minishell/test
pwd
mkdir hi
cd /tmp/dir.minishell/test/hi
pwd
cd /tmp/dir.minishell/test
pwd
EOF
expecting <<EOF
/tmp/dir.minishell/test
/tmp/dir.minishell/test/hi
/tmp/dir.minishell/test
EOF

when_run <<EOF "cd will print error when going into a directory with no access"
mkdir test
chmod -x test
cd test
pwd
chmod +x test
EOF
expecting <<EOF
minishell: cd: Permission denied
/tmp/dir.minishell
EOF

EXTRAENV="HOME=/tmp/dir.minishell/fakehome"
when_run <<EOF "cd with no arguments goes to \$HOME"
mkdir fakehome
cd
pwd
EOF
expecting <<EOF
/tmp/dir.minishell/fakehome
EOF

EXTRAENV="-u HOME"
when_run <<EOF "cd with no arguments, with HOME unset, prints error message and does nothing"
cd
pwd
EOF
expecting <<EOF
minishell: cd: HOME not set
/tmp/dir.minishell
EOF

when_run <<EOF "cd with empty argument"
cd ""
pwd
EOF
expecting <<EOF
/tmp/dir.minishell
EOF

EXTRAENV="HOME="
when_run <<EOF "cd with no arguments and HOME equal to empty string"
cd
pwd
EOF
expecting <<EOF
/tmp/dir.minishell
EOF

when_run <<EOF "cd with simple relative paths"
mkdir test
cd test
pwd
cd ..
pwd
EOF
expecting <<EOF
/tmp/dir.minishell/test
/tmp/dir.minishell
EOF

when_run <<EOF "cd with complicated relative paths"
mkdir -p test/hello/there
mkdir -p test/hi/there
mkdir -p test/hello/world
cd test/hello/there
pwd
cd ../../hi
pwd
cd ../hi/../hi/../hi/../hi
pwd
cd ../hi/../hi/../hi/../hi/../../test/hello/world
pwd
EOF
expecting <<EOF
/tmp/dir.minishell/test/hello/there
/tmp/dir.minishell/test/hi
/tmp/dir.minishell/test/hi
/tmp/dir.minishell/test/hello/world
EOF

when_run <<EOF "cd print error when given too many arguments"
cd hello there
pwd
cd . ..
pwd
EOF
expecting <<EOF
minishell: cd: too many arguments
/tmp/dir.minishell
minishell: cd: too many arguments
/tmp/dir.minishell
EOF

EXTRAENV="VAR=value hello=bye"
when_run <<"EOF" "variable substitution works"
echo $VAR $hello
EOF
expecting <<EOF
value bye
EOF

when_run <<"EOF" "export works with single variable"
export var=testvalue
echo var=$var
export var="Another Test Value"
echo var=$var
EOF
expecting <<EOF
var=testvalue
var=Another Test Value
EOF

when_run <<"EOF" "export works with multiple variables"
export name="Sir Reginald Pikedevant" greeting="Well met," counter=27 "question=How many fingers am I holding up?"
echo $greeting $name
echo $question
echo Umm, $counter?
EOF
expecting <<EOF
Well met, Sir Reginald Pikedevant
How many fingers am I holding up?
Umm, 27?
EOF

EXTRAENV=-i
when_run <<EOF "export with strange inputs"
export ENV=$(which env)
export PATH=$PATH
export var
echo status=\$?
echo var=[\$var]
export blue=
echo status=\$?
echo blue=[\$blue]
\$ENV -u PATH -u ENV env
EOF
expecting <<EOF
status=0
var=[]
status=0
blue=[]
blue=
EOF

when_run <<"EOF" "export with invalid identifiers"
export = 123=456 hello=hi 456=789
echo status=$?
echo hello=$hello
EOF
expecting <<"EOF"
minishell: export: `=': not a valid identifier
minishell: export: `123=456': not a valid identifier
minishell: export: `456=789': not a valid identifier
status=1
hello=hi
EOF

when_run <<EOF "empty command is not found"
""
EOF
expecting <<EOF
minishell: : command not found
EOF

when_run <<EOF "exit status of last command is preserved"
echo \$?
ls nonexist
echo \$?
echo hi
echo \$?
EOF
expecting <<EOF
0
ls: cannot access 'nonexist': No such file or directory
2
hi
0
EOF

when_run <<EOF "exit works"
$MINISHELL
exit
echo \$?
EOF
expecting <<EOF
0
EOF

when_run <<EOF "exit works with custom exit code"
$MINISHELL
exit 99
echo \$?
EOF
expecting <<EOF
99
EOF

when_run <<EOF "exit works with extreme exit codes"
$MINISHELL
exit 1234567890
echo \$?
$MINISHELL
exit -1234567890
echo \$?
EOF
expecting <<EOF
210
46
EOF

when_run <<EOF "exit works with nan input"
$MINISHELL
exit not-a-number
echo \$?
EOF
expecting <<EOF
minishell: exit: not-a-number: numeric argument required
2
EOF

when_run <<EOF "exit works with too many arguments"
$MINISHELL
exit 1 2
echo \$?
exit
echo \$?
$MINISHELL
exit not-a-number 3
echo \$?
$MINISHELL
exit 3 not-a-number
echo \$?
exit
echo \$?
EOF
expecting <<EOF
minishell: exit: too many arguments
1
0
minishell: exit: not-a-number: numeric argument required
2
minishell: exit: too many arguments
1
0
EOF

when_run <<EOF "echo basic tests"
echo hello world
echo "Hello    World"
EOF
expecting <<EOF
hello world
Hello    World
EOF

when_run <<EOF "echo -n tests"
echo -n hello world
echo
echo -nnnnnnnnnnnnnnnn hello world
echo
echo -nnnntnnnnnnnnnnnn hello world
echo hi -n hello
echo -n -n -nnn -n goodbye
echo
EOF
expecting <<EOF
hello world
hello world
-nnnntnnnnnnnnnnnn hello world
hi -n hello
goodbye
EOF

when_run <<EOF "echo - n"
echo - n hi
EOF
expecting <<EOF
- n hi
EOF

EXTRAENV="-i"
when_run <<EOF "env works"
echo this should be empty:
env
echo
export var=VALUE hi=hello bye=goodbye
echo this contains some values
env
unset var bye
echo some vars were unset
env
EOF
expecting <<EOF
this should be empty:

this contains some values
var=VALUE
hi=hello
bye=goodbye
some vars were unset
hi=hello
EOF

EXTRAENV="-i"
when_run <<EOF "env ignores additional arguments"
export var=VALUE hi=hello bye=goodbye
env -i name="The Other" ls
EOF
expecting <<EOF
minishell: env: ignoring arguments
var=VALUE
hi=hello
bye=goodbye
EOF

when_run <<EOF "echo field splitting tests"
export val="  hi   there   "
echo -\$val-
echo -"\$val"-
export val="hello   there"
echo -\$val-
echo -"\$val"-
EOF
expecting <<EOF
- hi there -
-  hi   there   -
-hello there-
-hello   there-
EOF

when_run <<EOF "wildcards"
echo *
touch hi there hello
echo *
echo h*
EOF
expecting <<EOF
*
hello hi there
hello hi
EOF

when_run <<EOF "quoted parentheses are not operators"
.debug
.exec
echo unclosed '('
EOF
expecting <<EOF
[dbg: 1]
[exec: 0]
parsed command
 ╰─ t_cmd
     ├─ t_cmd_type = FT_SIMPLE
     ├─ flags = 0
     ├─ line = 0
     ╰─ value
         ╰─ t_simple_cmd
             ├─ line = 0
             ├─ t_wordlist
             │   ├─ t_worddesc
             │   │   ├─ word   = [echo]
             │   │   ├─ marker = [    ]
             │   │   ├─ flags = 0
             │   │   ╰─ t_token_type = WORD_TOKEN
             │   ├─ t_worddesc
             │   │   ├─ word   = [unclosed]
             │   │   ├─ marker = [        ]
             │   │   ├─ flags = 0
             │   │   ╰─ t_token_type = WORD_TOKEN
             │   ╰─ t_worddesc
             │       ├─ word   = ['(']
             │       ├─ marker = [ ' ]
             │       ├─ flags = 2
             │       ╰─ t_token_type = WORD_TOKEN
             ╰─ redirections = (empty redir list)
EOF

when_run <<EOF "multiple >"
> > > >
echo \$?
EOF
expecting <<"EOF"
minishell: syntax error near unexpected token `>'
2
EOF

when_run <<EOF "single pipe char"
|
echo \$?
EOF
expecting <<"EOF"
minishell: syntax error near unexpected token `|'
2
EOF

when_run <<EOF "unclosed cmdgroup"
(echo hi
echo \$?
EOF
expecting <<"EOF"
minishell: syntax error near unexpected token `newline'
2
EOF

when_run <<EOF "basic pipe, second command does not accept input"
echo \$? | echo \$?
EOF
expecting <<EOF
0
EOF

when_run <<EOF "basic pipe"
echo hi | cat
EOF
expecting <<EOF
hi
EOF

when_run <<EOF "variable substitution with no valid name is not substitued"
echo \$=
echo \$:
EOF
expecting <<EOF
$=
$:
EOF

when_run <<EOF "variable substitution with a lone dollar sign"
echo " $ " | cat -e
EOF
expecting <<EOF
 $ $
EOF

when_run <<EOF "handling of positional arguments"
echo \$9HOME
EOF
expecting <<EOF
\$9HOME
EOF

when_run <<EOF "invalid redirection in echo"
echo < >
EOF
expecting <<"EOF"
minishell: syntax error near unexpected token `>'
EOF

when_run <<EOF "here_doc"
cat << EOT
hello!
EOT
EOF
expecting <<EOF
hello!
EOF

when_run <<EOF "double pipe after echo"
echo | |
EOF
expecting <<"EOF"
minishell: syntax error near unexpected token `|'
EOF

when_run <<EOF "empty variables are removed"
""
\$EMPTY_NO_VALUE
echo not \$EMPTY_NO_VALUE printed
EOF
expecting <<EOF
minishell: : command not found
not printed
EOF

when_run <<EOF "absolute path to command which does not exist"
/cmd/does/not/exist
echo \$?
EOF
expecting <<EOF
minishell: /cmd/does/not/exist: No such file or directory
127
EOF

when_run <<EOF "?\$HOME"
?\$HOME
echo \$?
EOF
expecting <<EOF
minishell: ?/home/khais: No such file or directory
127
EOF

when_run <<"EOF" "export with no arguments"
export
export $DONTEXIT
export $NOSUCHVAR $DONTEXIST
EOF
expecting <<EOF
EOF

when_run <<EOF "env with grep"
env | grep HOME
EOF
expecting <<EOF
$(env | grep HOME)
EOF

when_run <<EOF "export with empty arguments"
export "" ""
echo \$?
EOF
expecting <<"EOF"
minishell: export: `': not a valid identifier
minishell: export: `': not a valid identifier
1
EOF

when_run <<EOF "export with invalid identifiers"
export % $ 0 1 \$?
echo \$?
EOF
expecting <<"EOF"
minishell: export: `%': not a valid identifier
minishell: export: `$': not a valid identifier
minishell: export: `0': not a valid identifier
minishell: export: `1': not a valid identifier
minishell: export: `0': not a valid identifier
1
EOF

when_run <<"EOF" "export with identifiers containing numbers"
export VAR1=this VAR2=should VAR3=work VAR4=no VAR5="problem!"
echo $VAR1 $VAR2 $VAR3 $VAR4 $VAR5
EOF
expecting <<EOF
this should work no problem!
EOF

when_run <<"EOF" "export with identifiers containing and starting with underscore"
export __var__=__value__ _snake_case_variable_names_=Pascal_Snake_Case
echo $__var__ $_snake_case_variable_names_
EOF
expecting <<EOF
__value__ Pascal_Snake_Case
EOF

when_run <<"EOF" "export with identifiers containing dashes"
export -VAR=value lisp-case="(defun botsbuildbots () (botsbuildbots))" --option-case=--no-preserve-root
EOF
expecting <<"EOF"
minishell: export: `-VAR=value': not a valid identifier
minishell: export: `lisp-case=(defun botsbuildbots () (botsbuildbots))': not a valid identifier
minishell: export: `--option-case=--no-preserve-root': not a valid identifier
EOF

when_run <<"EOF" "export with variable substitution in arguments"
export type=nuclear phrase="the $type option"
echo $phrase
export phrase="the $type option" $type="the $type option"
echo $phrase
echo $nuclear
EOF
expecting <<EOF
the option
the nuclear option
the nuclear option
EOF

when_run <<"EOF" "export with variable substitution in arguments"
export type=nuclear phrase="the $type option"
echo $phrase
export phrase="the $type option" $type="the $type option"
echo $phrase
echo $nuclear
EOF
expecting <<EOF
the option
the nuclear option
the nuclear option
EOF

finalize
