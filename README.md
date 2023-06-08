# BookStore-api-cpp

Developed using [pistache](https://pistacheio.github.io/pistache/docs/rest-description/) and [rapidjson](https://rapidjson.org/). It uses `basic auth`. 

#### Build:

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
curl -XPOST -H "Content-type: application/json" -d '{
    "name": "BEG",
    "author": "GEB",
    "id": 12
}' 'http://localhost:9900/books'
```

##### Get a book record by id: 

```bash
curl -XGET 'http://localhost:9900/books/12'
```



