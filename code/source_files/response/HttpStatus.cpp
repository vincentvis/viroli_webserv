#include "response/HttpStatus.hpp"

const std::string &HttpStatus::get_template_start() const {
	static const std::string template_start =
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"	<meta charset=\"UTF-8\">\n"
		"	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"	<style type=\"text/css\">\n"
		"		"
		"html{line-height:1.15;-webkit-text-size-adjust:100%}body{margin:0}main{display:"
		"block}h1{font-size:2em;margin:0.67em\n"
		"0}hr{box-sizing:content-box;height:0;overflow:visible}pre{font-family:monospace,"
		"\n"
		"monospace;font-size:1em}a{background-color:transparent}abbr[title]{border-"
		"bottom:none;text-decoration:underline;text-decoration:underline\n"
		"dotted}b,strong{font-weight:bolder}code,kbd,samp{font-family:monospace,\n"
		"monospace;font-size:1em}small{font-size:80%}sub,sup{font-size:75%;line-height:0;"
		"position:relative;vertical-align:baseline}sub{bottom:-0.25em}sup{top:-0.5em}img{"
		"border-style:none}button,input,optgroup,select,textarea{font-family:inherit;"
		"font-size:100%;line-height:1.15;margin:0}button,input{overflow:visible}button,"
		"select{text-transform:none}[type=\"button\"],[type=\"reset\"],[type=\"submit\"],"
		"button{-webkit-appearance:button}[type=\"button\"]::-moz-focus-inner,[type="
		"\"reset\"]::-moz-focus-inner,[type=\"submit\"]::-moz-focus-inner,button::-moz-"
		"focus-inner{border-style:none;padding:0}[type=\"button\"]:-moz-focusring,[type="
		"\"reset\"]:-moz-focusring,[type=\"submit\"]:-moz-focusring,button:-moz-"
		"focusring{outline:1px\n"
		"dotted ButtonText}fieldset{padding:0.35em 0.75em\n"
		"0.625em}legend{box-sizing:border-box;color:inherit;display:table;max-width:100%;"
		"padding:0;white-space:normal}progress{vertical-align:baseline}textarea{overflow:"
		"auto}[type=\"checkbox\"],[type=\"radio\"]{box-sizing:border-box;padding:0}[type="
		"\"number\"]::-webkit-inner-spin-button,[type=\"number\"]::-webkit-outer-spin-"
		"button{height:auto}[type=\"search\"]{-webkit-appearance:textfield;outline-"
		"offset:-2px}[type=\"search\"]::-webkit-search-decoration{-webkit-appearance:"
		"none}::-webkit-file-upload-button{-webkit-appearance:button;font:inherit}"
		"details{display:block}summary{display:list-item}template{display:none}[hidden]{"
		"display:none}\n"
		"		body {\n"
		"			font-family: sans-serif;\n"
		"			background: black;\n"
		"			color: white;\n"
		"		}\n"
		"		article {\n"
		"			padding-top: 4vh;\n"
		"			max-width: 80vw;\n"
		"			margin: 0 auto;\n"
		"			text-align: center;\n"
		"		}\n"
		"		h1 {\n"
		"			font-size: 3em;\n"
		"		}\n"
		"	</style>\n"
		"	<title>";

	return template_start;
}

std::string HttpStatus::getFromTemplate(const std::string &code,
										const std::string &msg) const {
	std::string page = get_template_start() + code + " - " + msg +
					   "</title>\n"
					   "</head>\n"
					   "<body>\n"
					   "	<article>\n"
					   "		<h1>" +
					   code + " - " + msg +
					   "</h1>\n"
					   "		<img alt=\"" +
					   msg + "\" src=\"https://http.cat/" + code +
					   "\">\n"
					   "	</article>\n"
					   "</body>\n"
					   "</html>\n";

	return (page);
}
