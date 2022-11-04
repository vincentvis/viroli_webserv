#
# Makefile for tests for viroli webserv project
#

# make sure the webserver is running..

CLR="\033[33m"
RST="\033[0m"

can_post:
	@echo $(CLR)"Try putting a pinda in a postable directory"$(RST)
	curl -v --data-binary "@testfiles/pinda.jpg" localhost:8080/can_post/a_new_pinda.jpg

can_post_and_get:
	@echo $(CLR)"Try putting a pinda in a postable directory"$(RST)
	curl -v --data-binary "@testfiles/pinda.jpg" localhost:8080/can_post_and_get/a_new_pinda.jpg
	@echo $(CLR)"Try getting the pinda back"$(RST)
	curl -v localhost:8080/can_post_and_get/a_new_pinda.jpg > verify_pinda.jpg
	@echo $(CLR)"Opening the verification pinda"$(RST)
	open verify_pinda.jpg

can_get:
	@echo $(CLR)"Try get request to valid location (should recieve 'example response'"$(RST)
	curl -v localhost:8080/can_get/can_get.txt
	@echo $(CLR)"Try getting from a route but file does not exist"$(RST)
	curl -v localhost:8080/can_get/slfjdsklfj.jpg

can_delete:
	@echo $(CLR)"Try deleting a file that exists"$(RST)
	curl -v -X DELETE localhost:8080/can_delete/file1.txt
	@echo $(CLR)"Try deleting a file that does not exist"$(RST)
	curl -v -X DELETE localhost:8080/can_delete/file.txt

cannot_post:
	@echo $(CLR)"Try posting to a route where it is not allowed (expect 405 response)"$(RST)
	curl -v -d "post_data" localhost:8080/cannot_post/file.txt

cannot_get:
	@echo $(CLR)"Try getting from a route where it is not allowed, but the file also does not exist (expect 405 response)"$(RST)
	curl -v localhost:8080/cannot_get/alfjdklsfj.txt

can_post_and_delete:
	@echo $(CLR)"Post a file and delete it"$(RST)
	curl -v -d "this some file content" localhost:8080/can_post_and_delete/example_file.txt
	@printf $(CLR)
	sleep 5
	@echo "Try removing file"$(RST)
	curl -v -X DELETE localhost:8080/can_post_and_delete/example_file.txt

cgi_get:
	@echo $(CLR)"A GET request to a python script that adds 2 numbers"$(RST)
	curl -v "localhost:8080/cgi-bin/add.py?val1=44&val2=19"

cgi_post:
	@echo $(CLR)"A POST request to a python script that adds 2 numbers"$(RST)
	curl -v -d "val1=44&val2=19" "localhost:8080/cgi-bin/add.py"




# servernames
get_coda_com:
	@echo $(CLR)"Should get English page (default)"$(RST)
	curl -v localhost:8248
	@echo $(CLR)"Should get English page (with correct host header)"$(RST)
	curl -v --resolve koda.com:8248:127.0.0.1 koda.com:8248

get_coda_nl:
	@echo $(CLR)"Should get Dutch page"$(RST)
	curl -v --resolve koda.nl:8248:127.0.0.1 koda.nl:8248

get_coda_it:
	@echo $(CLR)"Should get Italian page"$(RST)
	curl -v --resolve koda.it:8248:127.0.0.1 koda.it:8248
