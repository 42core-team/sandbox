build: 
	cargo build --manifest-path core/Cargo.toml --bin game
	cargo build --manifest-path core/Cargo.toml --bin visualizer
	make -C martin

run: build
	# game start
	# sleep 1
	# start visualizer
	# start marting
	# ./starlord
