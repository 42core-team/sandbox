# Variables
PLAYER1_ID := 10
PLAYER2_ID := 20

# Folders
MARTIN := bot/martin
STARLORD := bot/starlord

# -------------------- Run targets --------------------
all: sandbox

# Starts up the game and connects it to the visualizer
sandbox: bot

# Here it shows the output of the your program without the visualizer
bot: stop build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	$(STARLORD)/starlord $(PLAYER1_ID) > /dev/null &
	$(MARTIN)/martin $(PLAYER2_ID)

battle: stop build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	chmod +x ./bot1
	chmod +x ./bot2
	./bot2 $(PLAYER1_ID) > /dev/null &
	./bot1 $(PLAYER2_ID)

debug: stop build
	cargo run --manifest-path core/Cargo.toml --bin game -- $(PLAYER1_ID) $(PLAYER2_ID) &
	$(STARLORD)/starlord $(PLAYER1_ID) > /dev/null &
	$(MARTIN)/martin $(PLAYER2_ID) > /dev/null

stop:
	@pkill game > /dev/null || true &
	@pkill martin > /dev/null || true &
	@pkill starlord > /dev/null || true

# Starts up the visualizer and connects it to the game
visualizer: visualizer_build
	cargo run --manifest-path core/Cargo.toml --bin visualizer


# -------------------- Build targets --------------------
build: game_build martin_build starlord_build

game_build:
	cargo build --manifest-path core/Cargo.toml --bin game

visualizer_build:
	cargo build --manifest-path core/Cargo.toml --bin visualizer

martin_build:
	make -C $(MARTIN)

starlord_build:
	make -C $(STARLORD)


# -------------------- Clean targets --------------------
clean:
	make -C $(MARTIN) clean
	make -C $(STARLORD) clean

fclean: clean
	make -C $(MARTIN) fclean
	make -C $(STARLORD) fclean


# -------------------- Update Repo from Github --------------------
update:
	git pull origin dev
	git submodule update --init --recursive
	git -C core checkout dev
	git -C core pull
	git -C bot/connection checkout dev
	git -C bot/connection pull
	@docker compose --project-directory=./.devcontainer pull

re: fclean all


# --------------- Build my-core-bot-dev-image --------------------
build-dev-image:
	docker build -t registry.coregame.de/core/core-event-0:latest -f ./.github/workflows/my-core-bot-Dockerfile .

build-server-image:
	docker build -t registry.coregame.de/core/game-server:latest -f ./.github/workflows/game-server-Dockerfile .


.PHONY: all build clean fclean re
