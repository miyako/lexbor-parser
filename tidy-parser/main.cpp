//
//  main.cpp
//  lexbor-parser
//
//  Created by miyako on 2025/09/09.
//

#include "lexbor-parser.h"

static void usage(void)
{
    fprintf(stderr, "Usage:  lexbor-parser -r -i in -o out -\n\n");
    fprintf(stderr, "text extractor for html documents\n\n");
    fprintf(stderr, " -%c path: %s\n", 'i' , "document to parse");
    fprintf(stderr, " -%c path: %s\n", 'o' , "text output (default=stdout)");
    fprintf(stderr, " %c: %s\n", '-' , "use stdin for input");
    fprintf(stderr, " -%c: %s\n", 'r' , "raw text output (default=json)");
    exit(1);
}

extern OPTARG_T optarg;
extern int optind, opterr, optopt;

#ifdef _WIN32
OPTARG_T optarg = 0;
int opterr = 1;
int optind = 1;
int optopt = 0;
int getopt(int argc, OPTARG_T *argv, OPTARG_T opts) {

    static int sp = 1;
    register int c;
    register OPTARG_T cp;
    
    if(sp == 1)
        if(optind >= argc ||
             argv[optind][0] != '-' || argv[optind][1] == '\0')
            return(EOF);
        else if(wcscmp(argv[optind], L"--") == NULL) {
            optind++;
            return(EOF);
        }
    optopt = c = argv[optind][sp];
    if(c == ':' || (cp=wcschr(opts, c)) == NULL) {
        ERR(L": illegal option -- ", c);
        if(argv[optind][++sp] == '\0') {
            optind++;
            sp = 1;
        }
        return('?');
    }
    if(*++cp == ':') {
        if(argv[optind][sp+1] != '\0')
            optarg = &argv[optind++][sp+1];
        else if(++optind >= argc) {
            ERR(L": option requires an argument -- ", c);
            sp = 1;
            return('?');
        } else
            optarg = argv[optind++];
        sp = 1;
    } else {
        if(argv[optind][++sp] == '\0') {
            sp = 1;
            optind++;
        }
        optarg = NULL;
    }
    return(c);
}
#define ARGS (OPTARG_T)L"i:o:-rh"
#else
#define ARGS "i:o:-rh"
#endif

struct Document {
    std::string type;
    std::string text;
};

static void document_to_json(Document& document, std::string& text, bool rawText) {
    
    if(rawText){
        text = document.text;
    }else{
        Json::Value documentNode(Json::objectValue);
        documentNode["type"] = document.type;
        documentNode["text"] = document.text;

        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";
        text = Json::writeString(writer, documentNode);
    }
}

static void extract_text(lxb_dom_node_t *node, std::string& text) {
    
    if (!node) return;

    if(node->type == LXB_DOM_NODE_TYPE_TEXT) {
        lxb_dom_text_t *text_node = lxb_dom_interface_text(node);
        if(text_node) {
            lxb_dom_character_data data = text_node->char_data;
            lexbor_str_t str = data.data;
            if (str.length >0) {
                text += std::string((const char *)str.data, str.length);
            }
        }
    } else if (node->type == LXB_DOM_NODE_TYPE_ELEMENT ||
               node->type == LXB_DOM_NODE_TYPE_DOCUMENT ||
               node->type == LXB_DOM_NODE_TYPE_DOCUMENT_FRAGMENT) {
        for (lxb_dom_node_t *child = node->first_child; child; child = child->next) {
            extract_text(child, text);
        }
    }
}

int main(int argc, OPTARG_T argv[]) {
        
    const OPTARG_T input_path  = NULL;
    const OPTARG_T output_path = NULL;
    
    std::vector<unsigned char>html_data(0);

    int ch;
    std::string text;
    bool rawText = false;
    
    while ((ch = getopt(argc, argv, ARGS)) != -1){
        switch (ch){
            case 'i':
                input_path  = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            case '-':
            {
                _fseek(stdin, 0, SEEK_END);
                size_t len = (size_t)_ftell(stdin);
                _fseek(stdin, 0, SEEK_SET);
                html_data.resize(len);
                fread(html_data.data(), 1, html_data.size(), stdin);
            }
                break;
            case 'r':
                rawText = true;
                break;
            case 'h':
            default:
                usage();
                break;
        }
    }
        
    if((!html_data.size()) && (input_path != NULL)) {
        FILE *f = _fopen(input_path, _rb);
        if(f) {
            _fseek(f, 0, SEEK_END);
            size_t len = (size_t)_ftell(f);
            _fseek(f, 0, SEEK_SET);
            html_data.resize(len);
            fread(html_data.data(), 1, html_data.size(), f);
            fclose(f);
        }
    }
    
    if(!html_data.size()) {
        usage();
    }
    
    Document document;
    
    lxb_html_document_t *_document = lxb_html_document_create();
    if(_document) {
        lxb_status_t status = lxb_html_document_parse(_document, (const lxb_char_t*)html_data.data(), html_data.size());
        if (status == LXB_STATUS_OK) {
            lxb_html_body_element_t *body_element = lxb_html_document_body_element(_document);
            if(body_element) {
                document.type = "html";
                lxb_dom_node_t *body_node = (lxb_dom_node_t *)body_element;
                extract_text(body_node, document.text);
            }
        }
        lxb_html_document_destroy(_document);
    }
    
    document_to_json(document, text, rawText);
        
    if(!output_path) {
        std::cout << text << std::endl;
    }else{
        FILE *f = _fopen(output_path, _wb);
        if(f) {
            fwrite(text.c_str(), 1, text.length(), f);
            fclose(f);
        }
    }

    end:
        
    return 0;
}
