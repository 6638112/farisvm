#include "farisvm.hpp"
#include <cstdlib>
#include <iostream>

#include "mongo/client/dbclient.h"

int
main(int argc, char *argv[])
{
    farisvm vm;
    using namespace std;
    
    mongo::client::initialize();          

    const char CR = '\r';
    const char LF = '\n';

    static int counter = 0;
    
    // add filtering rules
    // vm.add_rule("||example.com^index", "filter1.txt");
    // vm.add_rule(".swf|", "filter2.txt");

    mongo::client::initialize();

    try {

      mongo::DBClientConnection client;
      client.connect( "localhost" );

      /* database: test, collection: users */
      const string ns = "farisvm.url";
      
      cout << "--- find ---" << endl;
      auto_ptr<mongo::DBClientCursor> cursor =
	client.query(ns, mongo::BSONObj());

      std::vector<farisvm::match_result> result[15000];
      farisvm::query_uri query[15000];

      query[0].set_uri("https://www.google.com/", "http://referer.com/");
      query[1].set_uri("http://example.com/index.html", "http://referer.com/");
      query[2].set_uri("http://example.com/index.swf", "http://referer.com/");
      
      counter = 3;
      while(cursor->more()) {
	mongo::BSONObj p = cursor->next();
	mongo::OID oid = p["_id"].OID(); 
	string url = p["url"].str();
	cout << "url: " << url;

	query[counter].set_uri(url, "http://referer.com/");
	counter++;

	if(counter = 100)
	  break;
      }

      const string ns2 = "farisvm.adblock";
      
      cout << "--- find2 ---" << endl;
      auto_ptr<mongo::DBClientCursor> cursor2 =
	client.query(ns2, mongo::BSONObj());

      counter = 0;
      while(cursor2->more()) {
	mongo::BSONObj p2 = cursor2->next();
	mongo::OID oid = p2["_id"].OID(); 
	std::string pattern = p2["pattern"].str();
	cout << "pattern: " << pattern;

	std::string pattern2;
	for (std::string::const_iterator it = pattern2.begin();
	     it != pattern2.end(); ++it) {
	  if (*it != CR && *it != LF) {
	    pattern += *it;
	  }
	  
	} // while(cursor2->more()) {

	cout << "pattern re: " << pattern2;
	
	// vm.add_rule(".swf|", "filter2.txt");
	vm.add_rule(pattern2, "filter1.txt");
      }

    vm.add_rule("||example.com^index", "filter1.txt");
    vm.add_rule(".swf|", "filter2.txt");
    vm.add_rule(".ru", "filter2.txt");
        
    vm.match(result, query, 1500);

    for (int i = 0; i < 15000; i++) {
        std::cout << query[i].get_uri() << std::endl;
        for (auto ret: result[i]) {
            std::cout << "  rule: " << ret.rule
                      << "\n  file: " << ret.file << std::endl;
        }
        std::cout << std::endl;
    }

      
    } catch( const mongo::DBException &e ) {
      std::cout << "caught " << e.what() << std::endl;
    }                       

    // do matching

    /*
    std::vector<farisvm::match_result> result[3];
    farisvm::query_uri query[3];

    query[0].set_uri("https://www.google.com/", "http://referer.com/");
    query[1].set_uri("http://example.com/index.html", "http://referer.com/");
    query[2].set_uri("http://example.com/index.swf", "http://referer.com/");

    vm.match(result, query, 3);

    for (int i = 0; i < 3; i++) {
        std::cout << query[i].get_uri() << std::endl;
        for (auto ret: result[i]) {
            std::cout << "  rule: " << ret.rule
                      << "\n  file: " << ret.file << std::endl;
        }
        std::cout << std::endl;
    }
    */

    return 0;
}
