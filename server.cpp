#define RAPIDJSON_HAS_STDSTRING 1
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <iostream>

using namespace Pistache;

struct Book{
	int id;
	std::string name;
	std::string author;
};

std::map< int, Book > db;

void hello(const Rest::Request& request, Http::ResponseWriter response) 
{
    response.send(Http::Code::Ok, "world!");
}

void echo_get(const Rest::Request& req, Http::ResponseWriter resp) 
{
    std::string text = req.hasParam(":text") ? req.param(":text").as<std::string>() : "No parameter supplied.";
    resp.send(Http::Code::Ok, text);
}

void echo(const Rest::Request& req, Http::ResponseWriter resp) 
{
    rapidjson::Document doc;
    doc.Parse(req.body().c_str());
    std::string responseString = doc.HasMember("text") ? doc["text"].GetString() : "No text parameter supplied in JSON:\n" + req.body();
    resp.send(Http::Code::Ok, responseString);
}

void createBook(const Rest::Request& req, Http::ResponseWriter resp){
//	std::size_t id = req.param(":id").as<std::size_t>();
//	std::cout << id << "\n";
//

//	for(const auto &[HeaderKey, RawHeader] : req.headers().rawList())
//	        std::cout << HeaderKey << ": " << RawHeader.value() << std::endl;
	try{
		auto username = req.headers().getRaw("username").value();
		auto password = req.headers().getRaw("password").value();
		if(username != "admin" || password != "admin") { /*not admin*/
			resp.send(Http::Code::Forbidden, "You are not allowed to do this\n");
		}
		std::cout << username << " " << password  << std::endl;
	}catch(...){
		std::cout << "Unauthorized\n";
		resp.send(Http::Code::Unauthorized);
	}
	

	rapidjson::Document doc;
	doc.Parse(req.body().c_str());
	std::string name = doc["name"].GetString();
	std::string author = doc["author"].GetString();
	int id = doc["id"].GetInt();
	Book book;
	book.id = id, book.name = name, book.author = author;
	db[id] = book;
	auto bookn = db[id];
	std::cout << bookn.name << " " << bookn.author << " " << bookn.id << "\n";
	resp.send(Http::Code::Ok, "Book entry created\n", MIME(Application, Plain));
}

void getBooks(const Rest::Request& req, Http::ResponseWriter resp){
	using namespace rapidjson;
	const char* json = "{\"name\":\"Joe\",\"grade\":\"A\"}";
	Document jsonDoc;
	jsonDoc.SetObject();
	Value myArray(rapidjson::kArrayType);
	

	for(auto const& entry: db){
		Value objValue;
		objValue.SetObject();
		const char* v = entry.second.name.c_str(); 
		objValue.AddMember( "name",  entry.second.name, jsonDoc.GetAllocator() );
		objValue.AddMember( "author", entry.second.author, jsonDoc.GetAllocator() );
		objValue.AddMember( "id", entry.second.id, jsonDoc.GetAllocator() );

		myArray.PushBack(objValue, jsonDoc.GetAllocator() );
		std::cout << v << std::endl;
	
	}
	jsonDoc.AddMember("allbooks", myArray, jsonDoc.GetAllocator());
	jsonDoc.AddMember("total", db.size(), jsonDoc.GetAllocator());
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);
	std::cout << buffer.GetString() << std::endl;
	resp.send(Http::Code::Ok, buffer.GetString(), MIME(Application, Json));
}

void getBookById(const Rest::Request& req, Http::ResponseWriter resp){
	std::size_t id = req.param(":id").as<std::size_t>();
	if(db.find(id) == db.end()) {
		resp.send(Http::Code::Not_Found, "Not Found");
	}
	auto book = db[id];

	std::cout << book.name << " " << book.author << " " << book.id << "\n";

	// serialize the book object (hard part)
	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	
	writer.StartObject();
	writer.Key("name");
	writer.String(book.name.c_str());
	writer.Key("author");
	writer.String(book.author.c_str());
	writer.Key("id");
	writer.Int(book.id);
	writer.EndObject();
	std::cout << strbuf.GetString() << std::endl;
	resp.send(Http::Code::Ok, strbuf.GetString(),  MIME(Application, Json));
}

int main(int argc, char* argv[]) 
{
    using namespace Rest;

    Router router;     
    Port port(9900);
    Address addr(Ipv4::any(), port);
    std::shared_ptr<Http::Endpoint> endpoint = std::make_shared<Http::Endpoint>(addr);
    auto opts = Http::Endpoint::options().threads(1);   // how many threads for the server
    endpoint->init(opts);

    Routes::Get(router, "/hello", Routes::bind(&hello));
    Routes::Post(router, "/echo", Routes::bind(&echo));
    Routes::Post(router, "/books", Routes::bind(&createBook));
    Routes::Get(router, "/books", Routes::bind(&getBooks));
    Routes::Get(router, "/books/:id", Routes::bind(&getBookById));
    Routes::Get(router, "/echo_get/:text?", Routes::bind(&echo_get));

    endpoint->setHandler(router.handler());
    endpoint->serve();
}
