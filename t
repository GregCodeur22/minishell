r>t
| cat
minishell> >out |cat
<<eof echo -nnnnnnnn >out gfdjhk -n //pas de redir
<<un
<<un<<deux<<trois cat
cat |<<un<<deux cat
garside@c2r2p6:~$ >| cat
garside@c2r2p6:~$ > | cat
bash: syntax error near unexpected token `|'
garside@c2r2p6:~$ 
garside@c2r2p6:~$ | > out cat
bash: syntax error near unexpected token `|'
garside@c2r2p6:~$ 
