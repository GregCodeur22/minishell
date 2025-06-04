# Compilation
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I includes/ -g3

# Couleurs
RESET       = \033[0m
GREEN       = \033[32m
RED         = \033[31m
YELLOW      = \033[33m
CYAN        = \033[36m
MAGENTA     = \033[35m

# Répertoires
SRC_DIR     = src/
OBJ_DIR     = obj/
OCTOLIB     = octolib

# Bibliothèque OctoLib (libft.a)
OCTOLIB_A   = $(OCTOLIB)/libft.a

# Sources et objets
SRC         = $(wildcard src/*.c src/parsing/*.c src/builtin/*.c src/exec/*.c)
OBJ         = $(patsubst src/%.c, obj/%.o, $(SRC))

# Nom de l'exécutable
NAME        = minishell

# =======================================
#             RÈGLES MAKE
# =======================================

all: $(NAME)

# Compilation de libft (OctoLib)
$(OCTOLIB_A):
	@$(MAKE) -C $(OCTOLIB)

# Création de l'exécutable minishell
$(NAME): $(OBJ) | $(OCTOLIB_A)
	@$(CC) $(CFLAGS) $(OBJ) $(OCTOLIB_A) -o $(NAME) -lreadline
	@echo "$(GREEN)✅ Compilation terminée : $(NAME)$(RESET)"

# Compilation des fichiers objets
obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CYAN)🛠️ Compilation de $<$(RESET)"

# Nettoyage des fichiers objets
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(OCTOLIB) clean
	@echo "$(YELLOW)🧹 Suppression des fichiers objets$(RESET)"

# Nettoyage complet (objets + executable)
fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(OCTOLIB) fclean
	@echo "$(RED)❌ Suppression de l'exécutable $(NAME) et des fichiers objets$(RESET)"

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re
