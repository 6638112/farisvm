# flockGazer leveraged by FARIS

This repository is a sophisticated version of flockGazer which is leveraged by FARIS for URL classificaion.

# FARIS: the FAst uniform Resource Identifier-Specific filter

FARIS which stands for the FAst uniform Resource Identifier-Specific filter is a fundamental technology for
intrusion detection, HTTP proxies,
content distribution networks, content-centric networks, and many other application areas.

FARIS applies AdBlock Plus which can be improved in that a slow application that consumes much memory.

## How to Use

```c
#include "farisvm.hpp"
#include <iostream>

int
main(int argc, char *argv[])
{
    farisvm vm; // create a instance of FARIS

    // add filter rules
    //
    // add_rule(filter, filename)
    //    filter: filter rule of AdBlock Plus
    //    filename: file name of the filter rule
    vm.add_rule("||example.com^index", "filter1.txt");
    vm.add_rule(".swf|", "filter2.txt");

    std::vector<farisvm::match_result> result[3]; // a vector in which the results are stored
    farisvm::query_uri query[3]; // a query object

    // set urls for query object
    //
    // set_uri(uri, referer)
    //     uri: URI
    //     referer: HTTP referer
    query[0].set_uri("https://www.google.com/", "http://referer.com/");
    query[1].set_uri("http://example.com/index.html", "http://referer.com/");
    query[2].set_uri("http://example.com/index.swf", "http://referer.com/");

    // do matching
    //
    // match(result, query, query_num)
    //     result: the results of matching
    //     query: URIs to be matched
    //     query_num: the number or URIs
    vm.match(result, query, 3);

    for (int i = 0; i < 3; i++) {
        std::cout << query[i].get_uri() << std::endl;
        for (auto ret: result[i]) {
            std::cout << "  rule: " << ret.rule                 // print a matched rule
                      << "\n  file: " << ret.file << std::endl; // print the file name of the rule
        }
        std::cout << std::endl;
    }

    return 0;
}
```

## Publication

Yuuki Takano and Ryosuke Miura, "FARIS: Fast and Memory-efficient URL Filter by Domain Specific Machine", IEEE International Conference on IT Convergence and Security 2016 (ICITCS 2016), Sep. 2016, ISBN 987-1-5090-3764-3, pp. 204-210.

# measurement

registering urls
<xamp>
# python out-url-2.py url-track-2014-08-27-01-drem
</xamp>

<pre>
# pwd
/home/flare/farisvm/tools

# more insert.sh
python insert-pattern.py easylist_france.txt
python insert-pattern.py easylist_germany.txt
python insert-pattern.py easylist_italy.txt
python insert-pattern.py easylist.txt
python insert-pattern.py easyprivacy.txt
python insert-pattern.py fanboy_annoyance.txt
python insert-pattern.py japanese_tofu.txt
python insert-pattern.py japanese.txt
python insert-pattern.py malwaredomains_full.txt

</pre>

match.

<pre>
~/farisvm/src# ./match
--- find ---
rules total: 15
--- find ---
apk-url total: 487363

</pre>