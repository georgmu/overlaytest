
tmpfiletest: tmpfiletest.c

overlaytest: tmpfiletest
	./run_overlaytest.sh

dockertest: tmpfiletest
	docker run --rm $$(docker build -q .)
