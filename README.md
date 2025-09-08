![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/lexbor-parser)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/lexbor-parser/total)

### Dependencies and Licensing

* the source code of this CLI tool is licensed under the MIT license.
* see [lexbor](https://github.com/lexbor/lexbor/blob/master/LICENSE) for the licensing of **lexbor** (Apache 2.0).
 
# lexbor-parser
CLI tool to extract text from HTML

## usage

```
lexbor-parser -i example.html -o example.json

 -i path    : document to parse
 -o path    : text output (default=stdout)
 -          : use stdin for input
 -r         : raw text output (default=json)
```

## output (JSON)

```
{
    "type: "html",
    "text": "body"
}
```
