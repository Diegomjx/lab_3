Schedulers: 
	gcc -lpthread ./Schedulers.c -o ./Schedulers && ./Schedulers

clean: 
	rm -f ./Schedulers

run: 
	rm -f ./Schedulers && gcc -lpthread ./Schedulers.c -o ./Schedulers && ./Schedulers