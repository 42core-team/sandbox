# Variables
PLAYER1_ID := 1
PLAYER2_ID := 2

# Folders
BOT := bot

# -------------------- Run targets --------------------
all: sandbox

# Starts up the game and connects it to the visualizer
sandbox: stop build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	./starlord $(PLAYER1_ID) > /dev/null &
	./bot/bot $(PLAYER2_ID) > /dev/null &
	cargo run --manifest-path core/Cargo.toml --bin visualizer
	# Run visualizer

# Here it shows the output of the your program without the visualizer
debug: stop build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	./starlord $(PLAYER1_ID) > /dev/null &
	./bot/bot $(PLAYER2_ID)

debug_game: stop build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID) &
	./starlord $(PLAYER1_ID) > /dev/null &
	./bot/bot $(PLAYER2_ID) > /dev/null

stop:
	@pkill game > /dev/null || true &
	@pkill bot > /dev/null || true &
	@pkill starlord > /dev/null || true

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
	make -C $(BOT)



# -------------------- Clean targets --------------------
clean:
	cargo clean --manifest-path core/Cargo.toml
	make -C $(BOT) clean

fclean: clean
	rm -rf target
	make -C $(BOT) fclean

# -------------------- Update Repo from Github --------------------
update:
	git pull origin master
	git submodule update --init --recursive
	git -C core checkout main
	git -C core pull
	git -C bot/connection checkout main
	git -C bot/connection pull

re: fclean all

.PHONY: all build clean fclean re
