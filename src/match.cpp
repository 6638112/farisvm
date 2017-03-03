#include "farisvm.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>

#include "mongo/client/dbclient.h"

#define CNTR 700000
// #define CNTR 498970

using namespace std;

string string_trim(const string& str, const char* delim=" \t\r\n");
string string_trim(const string& str, const char* delim)
{
  const int p1(str.find_first_not_of(delim));
  if(p1 == string::npos){
    return string();
  }
  const int p2(str.find_last_not_of(delim));
  return str.substr(p1, p2 - p1 + 1);
}

int
main(int argc, char *argv[])
{
    farisvm vm;
        
    mongo::client::initialize();          

    const char CR = '\r';
    const char LF = '\n';

    static int counter = 0;
    static int all = 0;
    
    mongo::client::initialize();

    try {

      mongo::DBClientConnection client;
      client.connect( "localhost" );

      /* database: test, collection: users */
      const string ns2 = "farisvm.adblock";
      
      cout << "--- find ---" << endl;
      auto_ptr<mongo::DBClientCursor> cursor2 =
	client.query(ns2, mongo::BSONObj());

      static std::vector<farisvm::match_result> result[CNTR];
      static farisvm::query_uri query[CNTR];
      
      counter = 3;
      string url2;

      while(cursor2->more()) {
	mongo::BSONObj p = cursor2->next();
	mongo::OID oid = p["_id"].OID(); 
	string url = p["pattern"].str();

	url2 = string_trim(string(url));
	vm.add_rule(url2, "rule");
	
	// query[counter].set_uri(url2, "http://referer.com/");
	counter++;
	all++;
	
	if(counter > CNTR-1)
	  break;
      }

      cout << "rules total: " << all << std::endl;

      /* 
	 database: farisvm, collection: apkurl 
       */
      const string ns = "farisvm.apkurl";
      
      cout << "--- find ---" << endl;
      auto_ptr<mongo::DBClientCursor> cursor =
	client.query(ns, mongo::BSONObj());

      query[0].set_uri("https://www.google.com/", "http://referer.com/");
      query[1].set_uri("http://example.com/index.html", "http://referer.com/");
      query[2].set_uri("http://example.com/index.swf", "http://referer.com/");
      
      counter = 3;

      while(cursor->more()) {
	mongo::BSONObj p = cursor->next();
	mongo::OID oid = p["_id"].OID(); 
	string url = p["url"].str();

	url2 = string_trim(string(url));
	query[counter].set_uri(url2, "http://referer.com/");
	// vm.add_rule(url2, counter);
	
	counter++;
	all++;
	
	if(counter > CNTR-1)
	  break;
      }

    cout << "apk-url total: " << all << std::endl;
      
    vm.add_rule("*.go.*", "1");
    vm.add_rule("*gov.*", "2");
    vm.add_rule("*swf|", "3");

    std::chrono::system_clock::time_point  start, end;
    
    start = std::chrono::system_clock::now();
    vm.match(result, query, CNTR-1);
    end = std::chrono::system_clock::now(); 
    // double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    double elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start).count(); 

    cout << "processing time:" << elapsed << endl;
    
    /*
    for (int i = 0; i < CNTR-1; i++) {
        
        for (auto ret: result[i]) {

	  if(!ret.rule.empty() && !ret.file.empty())
	    {
	     std::cout << query[i].get_uri() << std::endl;
	     std::cout << "  rule: " << ret.rule
                      << "\n  file: " << ret.file << std::endl;
	    }
        }

       if (i%30000 == 0)
	  cout << "counter:" << i/30000 << endl;
    }
    */
      
    } catch( const mongo::DBException &e ) {
      std::cout << "caught " << e.what() << std::endl;
    }                       

    return 0;
}
