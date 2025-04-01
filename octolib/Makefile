# ============================
#         Variables
# ============================

NAME        = libft.a
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -I includes/
AR          = ar rcs
RM          = rm -rf

# Répertoires
SRC_DIR     = libft/
OBJ_DIR     = obj/

# Fichiers sources
SRC         = $(wildcard $(SRC_DIR)*.c)
BONUS       = $(wildcard $(SRC_DIR)*_bonus.c)

# Transformation des chemins
OBJ         = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
OBJ_BONUS   = $(BONUS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# ============================
#         Règles Make
# ============================

all: $(NAME)

$(NAME): $(OBJ)
	@mkdir -p $(OBJ_DIR)
	@$(AR) $(NAME) $(OBJ)
	@echo "✅ Compilation terminée : $(NAME)"

# Compilation des fichiers objets
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "🛠️  Compilation de $<"

# Compilation du bonus
bonus: $(OBJ) $(OBJ_BONUS)
	@mkdir -p $(OBJ_DIR)
	@$(AR) $(NAME) $(OBJ) $(OBJ_BONUS)
	@echo "✨ Compilation du bonus terminée !"

# Nettoyage des fichiers objets
clean:
	@$(RM) $(OBJ_DIR)
	@echo "🧹 Suppression des fichiers objets."

# Nettoyage complet
fclean: clean
	@$(RM) $(NAME)
	@echo "❌ Suppression de la bibliothèque."

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re bonus
