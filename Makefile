# Variables
PLAYER1_ID := 10
PLAYER2_ID := 20

# Folders
MARTIN := bot/martin
GRIDMASTER := bot/gridmaster

# -------------------- Run targets --------------------
all: build
re: fclean all
ren: fclean run

bot: run
start: run

run: stop build
	./core/core $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	$(GRIDMASTER)/gridmaster $(PLAYER1_ID) > /dev/null &
	$(MARTIN)/martin $(PLAYER2_ID)

battle: stop build
	chmod +x ./bot1
	chmod +x ./bot2
	./core/core $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	./bot2 $(PLAYER1_ID) > /dev/null &
	./bot1 $(PLAYER2_ID)

debug: stop build
	$(GRIDMASTER)/gridmaster $(PLAYER1_ID) &
	$(MARTIN)/martin $(PLAYER2_ID) &
	./core/core $(PLAYER1_ID) $(PLAYER2_ID)
rebug: fclean debug # re but for debug

stop:
	@pkill game > /dev/null || true &
	@pkill martin > /dev/null || true &
	@pkill gridmaster > /dev/null || true


# -------------------- Build targets --------------------
build: game_build martin_build gridmaster_build

game_build:
	make -C core

martin_build:
	make -C $(MARTIN)

gridmaster_build:
	make -C $(GRIDMASTER)


# -------------------- Clean targets --------------------
clean: stop
	make -C $(MARTIN) clean
	make -C $(GRIDMASTER) clean
	make -C core clean

fclean: clean
	make -C $(MARTIN) fclean
	make -C $(GRIDMASTER) fclean
	make -C core fclean


# -------------------- Update Repo from Github --------------------
update:
	git pull origin dev
	git submodule update --init --recursive
	git -C core checkout dev
	git -C core pull
	git -C bot/connection checkout dev
	git -C bot/connection pull
	@docker compose --project-directory=./.devcontainer pull
	make -C core update


# --------------- Build my-core-bot-dev-image --------------------
build-dev-image:
	docker build -t registry.coregame.de/core/core-event-01:latest -f ./.github/workflows/my-core-bot-Dockerfile .

build-dev-image-multiarch:
	docker build -t registry.coregame.de/core/core-event-01:latest --platform linux/amd64,linux/arm64/v8 -f ./.github/workflows/my-core-bot-Dockerfile .

build-server-image:
	docker build -t registry.coregame.de/core/game-server:latest -f ./.github/workflows/game-server-Dockerfile .


.PHONY: all re bot start run ren battle debug rebug stop build game_build martin_build gridmaster_build clean fclean update build-dev-image build-dev-image-multiarch build-server-image
