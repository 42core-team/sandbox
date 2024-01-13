# Variables
PLAYER1_ID := 1
PLAYER2_ID := 2

# Folders
MARTIN := martin

# -------------------- Run targets --------------------
all: sandbox

# Starts up the game and connects it to the visualizer
sandbox: build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID)
	# Run PLAYER1_ID
	# Run PLAYER2_ID
	# Run visualizer

# Here it shows the output of the your program without the visualizer
debug: build

# Starts up the visualizer and connects it to the game
visualizer: visualizer_build
	cargo run --manifest-path core/Cargo.toml --bin visualizer



# -------------------- Build targets --------------------
build: game_build visualizer_build martin_build

game_build: 
	cargo build --manifest-path core/Cargo.toml --bin game

visualizer_build:
	cargo build --manifest-path core/Cargo.toml --bin visualizer

martin_build:
	make -C $(MARTIN)



# -------------------- Clean targets --------------------
clean: 
	cargo clean --manifest-path core/Cargo.toml
	make -C $(MARTIN) clean

fclean: clean
	rm -rf target
	make -C $(MARTIN) fclean

re: fclean all

.PHONY: all build clean fclean re