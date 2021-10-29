PAR = 1

tmpfiletest: tmpfiletest.c

overlaytest: tmpfiletest
	./run_overlaytest.sh ${PAR}

dockertest: tmpfiletest
	docker run --rm $$(docker build -q .)
