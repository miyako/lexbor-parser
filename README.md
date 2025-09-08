![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/tidy-parser)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/tidy-parser/total)

### Dependencies and Licensing

* the source code of this CLI tool is licensed under the MIT license.
* see [tidy-html5](https://github.com/htacg/tidy-html5/blob/next/README/LICENSE.md) for the licensing of **tidy-html5** (W3C).
 
# pdfium-parser
CLI tool to extract text from HTML

## usage

```
tidy-parser -i example.html -o example.json

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
