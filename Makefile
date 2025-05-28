# Compilation
CC          = cc
CFLAGS     = -Wall -Wextra -Werror -I includes/ -g3

#-fsanitize=address

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
SRC         = $(wildcard $(SRC_DIR)*.c)
OBJ         = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

# Nom de l'exécutable
NAME        = minishell

# =======================================
#             RÈGLES MAKE
# =======================================

# Compilation principale
all: $(NAME)

# Compilation de libft (OctoLib)
$(OCTOLIB_A):
	@$(MAKE) -C $(OCTOLIB)

# Création de l'exécutable minishell
$(NAME): $(OBJ) | $(OCTOLIB_A)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(OBJ) $(OCTOLIB_A) -o $(NAME) -lreadline
	@echo "$(GREEN)✅ Compilation terminée : $(NAME)$(RESET)"

# Compilation des fichiers objets
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CYAN)🛠️ Compilation de $<$(RESET)"

# Création du dossier obj/ si nécessaire
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Nettoyage des fichiers objets
clean:
	@rm -rf $(OBJ_DIR)
	#@$(MAKE) -C $(OCTOLIB) clean
	make clean -C $(OCTOLIB)
	@echo "$(YELLOW)🧹 Suppression des fichiers objets$(RESET)."

# Nettoyage complet (fichiers objets + exécutable)
fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(OCTOLIB) fclean
	@echo "$(RED)❌ Suppression de l'exécutable $(NAME) et des fichiers objets$(RESET)."

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re