#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct status {
		int   code;
		char *msg;
};

#define TEMPLATE \
	"<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
	<meta charset=\"UTF-8\">\n\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
	<style type=\"text/css\">\n\
		html{line-height:1.15;-webkit-text-size-adjust:100%%}body{margin:0}main{display:block}h1{font-size:2em;margin:0.67em\n\
0}hr{box-sizing:content-box;height:0;overflow:visible}pre{font-family:monospace,\n\
monospace;font-size:1em}a{background-color:transparent}abbr[title]{border-bottom:none;text-decoration:underline;text-decoration:underline\n\
dotted}b,strong{font-weight:bolder}code,kbd,samp{font-family:monospace,\n\
monospace;font-size:1em}small{font-size:80%%}sub,sup{font-size:75%%;line-height:0;position:relative;vertical-align:baseline}sub{bottom:-0.25em}sup{top:-0.5em}img{border-style:none}button,input,optgroup,select,textarea{font-family:inherit;font-size:100%%;line-height:1.15;margin:0}button,input{overflow:visible}button,select{text-transform:none}[type=\"button\"],[type=\"reset\"],[type=\"submit\"],button{-webkit-appearance:button}[type=\"button\"]::-moz-focus-inner,[type=\"reset\"]::-moz-focus-inner,[type=\"submit\"]::-moz-focus-inner,button::-moz-focus-inner{border-style:none;padding:0}[type=\"button\"]:-moz-focusring,[type=\"reset\"]:-moz-focusring,[type=\"submit\"]:-moz-focusring,button:-moz-focusring{outline:1px\n\
dotted ButtonText}fieldset{padding:0.35em 0.75em\n\
0.625em}legend{box-sizing:border-box;color:inherit;display:table;max-width:100%%;padding:0;white-space:normal}progress{vertical-align:baseline}textarea{overflow:auto}[type=\"checkbox\"],[type=\"radio\"]{box-sizing:border-box;padding:0}[type=\"number\"]::-webkit-inner-spin-button,[type=\"number\"]::-webkit-outer-spin-button{height:auto}[type=\"search\"]{-webkit-appearance:textfield;outline-offset:-2px}[type=\"search\"]::-webkit-search-decoration{-webkit-appearance:none}::-webkit-file-upload-button{-webkit-appearance:button;font:inherit}details{display:block}summary{display:list-item}template{display:none}[hidden]{display:none}\n\
		body {\n\
			font-family: sans-serif;\n\
			background: black;\n\
			color: white;\n\
		}\n\
		article {\n\
			padding-top: 4vh;\n\
			max-width: 80vw;\n\
			margin: 0 auto;\n\
			text-align: center;\n\
		}\n\
		h1 {\n\
			font-size: 3em;\n\
		}\n\
	</style>\n\
	<title>%1$i - %2$s</title>\n\
</head>\n\
<body>\n\
	<article>\n\
		<h1>%1$i - %2$s</h1>\n\
		<img alt=\"%2$s\" src=\"https://http.cat/%1$i\">\n\
	</article>\n\
</body>\n\
</html>\n"

int main(void) {
	struct status statusses[] = {
		{.code = 100, .msg = "Continue"},
		{.code = 101, .msg = "Switching Protocols"},
		{.code = 102, .msg = "Processing"},
		{.code = 200, .msg = "OK"},
		{.code = 201, .msg = "Created"},
		{.code = 202, .msg = "Accepted"},
		{.code = 203, .msg = "Non-Authoritative Information"},
		{.code = 204, .msg = "No Content"},
		{.code = 206, .msg = "Partial Content"},
		{.code = 207, .msg = "Multi-Status"},
		{.code = 300, .msg = "Multiple Choices"},
		{.code = 301, .msg = "Moved Permanently"},
		{.code = 302, .msg = "Found"},
		{.code = 303, .msg = "See Other"},
		{.code = 304, .msg = "Not Modified"},
		{.code = 305, .msg = "Use Proxy"},
		{.code = 307, .msg = "Temporary Redirect"},
		{.code = 308, .msg = "Permanent Redirect"},
		{.code = 400, .msg = "Bad Request"},
		{.code = 401, .msg = "Unauthorized"},
		{.code = 402, .msg = "Payment Required"},
		{.code = 403, .msg = "Forbidden"},
		{.code = 404, .msg = "Not Found"},
		{.code = 405, .msg = "Method Not Allowed"},
		{.code = 406, .msg = "Not Acceptable"},
		{.code = 407, .msg = "Proxy Authentication Required"},
		{.code = 408, .msg = "Request Timeout"},
		{.code = 409, .msg = "Conflict"},
		{.code = 410, .msg = "Gone"},
		{.code = 411, .msg = "Length Required"},
		{.code = 412, .msg = "Precondition Failed"},
		{.code = 413, .msg = "Payload Too Large"},
		{.code = 414, .msg = "Request-URI Too Long"},
		{.code = 415, .msg = "Unsupported Media Type"},
		{.code = 416, .msg = "Request Range Not Satisfiable"},
		{.code = 417, .msg = "Expectation Failed"},
		{.code = 418, .msg = "I’m a teapot"},
		{.code = 420, .msg = "Enhance Your Calm"},
		{.code = 421, .msg = "Misdirected Request"},
		{.code = 422, .msg = "Unprocessable Entity"},
		{.code = 423, .msg = "Locked"},
		{.code = 424, .msg = "Failed Dependency"},
		{.code = 425, .msg = "Too Early"},
		{.code = 426, .msg = "Upgrade Required"},
		{.code = 429, .msg = "Too Many Requests"},
		{.code = 431, .msg = "Request Header Fields Too Large"},
		{.code = 444, .msg = "No Response"},
		{.code = 450, .msg = "Blocked by Windows Parental Controls"},
		{.code = 451, .msg = "Unavailable For Legal Reasons"},
		{.code = 497, .msg = "HTTP Request Sent to HTTPS Port"},
		{.code = 498, .msg = "Token expired/invalid"},
		{.code = 499, .msg = "Client Closed Request"},
		{.code = 500, .msg = "Internal Server Error"},
		{.code = 501, .msg = "Not Implemented"},
		{.code = 502, .msg = "Bad Gateway"},
		{.code = 503, .msg = "Service Unavailable"},
		{.code = 504, .msg = "Gateway Timeout"},
		{.code = 506, .msg = "Variant Also Negotiates"},
		{.code = 507, .msg = "Insufficient Storage"},
		{.code = 508, .msg = "Loop Detected"},
		{.code = 509, .msg = "Bandwidth Limit Exceeded"},
		{.code = 510, .msg = "Not Extended"},
		{.code = 511, .msg = "Network Authentication Required"},
		{.code = 521, .msg = "Web Server Is Down"},
		{.code = 522, .msg = "Connection Timed Out"},
		{.code = 523, .msg = "Origin Is Unreachable"},
		{.code = 525, .msg = "SSL Handshake Failed"},
		{.code = 599, .msg = "Network Connect Timeout Error"},
		{.msg = NULL}};

	int i = 0;
	while (statusses[i].msg) {
		char filename[100];
		sprintf(filename, "../../website/html/error-pages/%i.html", statusses[i].code);

		FILE *file = fopen(filename, "w");
		if (file == NULL) {
			perror(filename);
			i++;
			continue;
		}
		fprintf(file, TEMPLATE, statusses[i].code, statusses[i].msg);
		fclose(file);
		i++;
	}

	return 0;
}
