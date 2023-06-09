# BookStore-api-cpp

Developed using [pistache](https://pistacheio.github.io/pistache/docs/rest-description/) and [rapidjson](https://rapidjson.org/). It uses `basic auth`. 

#### Build

```bash
g++ server.cpp base64.cpp -o server -lpistache
```

#### Run

```bash
./server 9900
```



#### Test

##### Create a book record: POST /books/

```bash
curl --location 'http://localhost:9900/books' \
--header 'Content-Type: application/json' \
--header 'Authorization: Basic YWRtaW46YWRtaW4=' \
--data '{
    "name": "The Great Gatsby",
    "author": "Fitzgerald"
}'
```

##### Get a book record by id 

```bash
curl -XGET 'http://localhost:9900/books/12'
```

##### Get all books

```bash
curl --location 'http://localhost:9900/books'
```

##### Delete a book by id

```bash
curl --location --request DELETE 'http://localhost:9900/books/1804289383' \
--header 'Authorization: Basic YWRtaW46YWRtaW4='
```

##### Update a book by id

```bash
curl --location --request PUT 'http://localhost:9900/books/2074039068' \
--header 'Content-Type: application/json' \
--header 'Authorization: Basic YWRtaW46YWRtaW4=' \
--data '{
    "name": "The Great Gatsby",
    "author": "Fitzgerald G."
}'
```



#####  



