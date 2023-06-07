#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "rapidjson/document.h"

#include <iostream>

using namespace Pistache;

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
	std::string id = req.param(":id").as<std::string>();
	std::cout << id << "\n";
	rapidjson::Document doc;
	doc.Parse(req.body().c_str());
	std::string name = doc["name"].GetString();
	std::cout << name << "\n";

	resp.send(Http::Code::Ok);
}

void getBooks(const Rest::Request& req, Http::ResponseWriter resp){
	
	resp.send(Http::Code::Ok);
}

int main(int argc, char* argv[]) 
{
    using namespace Rest;

    Router router;      // POST/GET/etc. route handler
    Port port(9900);    // port to listen on
    Address addr(Ipv4::any(), port);
    std::shared_ptr<Http::Endpoint> endpoint = std::make_shared<Http::Endpoint>(addr);
    auto opts = Http::Endpoint::options().threads(1);   // how many threads for the server
    endpoint->init(opts);

    Routes::Get(router, "/hello", Routes::bind(&hello));
    Routes::Post(router, "/echo", Routes::bind(&echo));
    Routes::Post(router, "/books/:id", Routes::bind(&createBook));
    Routes::Get(router, "/books", Routes::bind(&getBooks));
    Routes::Get(router, "/echo_get/:text?", Routes::bind(&echo_get));

    endpoint->setHandler(router.handler());
    endpoint->serve();
}
