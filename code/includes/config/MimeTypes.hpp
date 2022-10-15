#pragma once

#include <map>
#include <string>

class MimeTypes {
	public:
		static std::map<std::string, std::string> &getMap() {
			static MimeTypes instance;
			return instance._mimetypes;
		}

	private:
		std::map<std::string, std::string> _mimetypes;
		MimeTypes() {
			_mimetypes["3g2"]  = "video/3gpp2";
			_mimetypes["3gp"]  = "video/3gpp";
			_mimetypes["7z"]   = "application/x-7z-compressed";
			_mimetypes["aac"]  = "audio/aac";
			_mimetypes["abw"]  = "application/x-abiword";
			_mimetypes["arc"]  = "application/x-freearc";
			_mimetypes["avi"]  = "video/x-msvideo";
			_mimetypes["avif"] = "image/avif";
			_mimetypes["azw"]  = "application/vnd.amazon.ebook";
			_mimetypes["bin"]  = "application/octet-stream";
			_mimetypes["bmp"]  = "image/bmp";
			_mimetypes["bz"]   = "application/x-bzip";
			_mimetypes["bz2"]  = "application/x-bzip2";
			_mimetypes["cda"]  = "application/x-cdf";
			_mimetypes["csh"]  = "application/x-csh";
			_mimetypes["css"]  = "text/css";
			_mimetypes["csv"]  = "text/csv";
			_mimetypes["doc"]  = "application/msword";
			_mimetypes["docx"] =
				"application/vnd.openxmlformats-officedocument.wordprocessingml.document";
			_mimetypes["eot"]    = "application/vnd.ms-fontobject";
			_mimetypes["epub"]   = "application/epub+zip";
			_mimetypes["gif"]    = "image/gif";
			_mimetypes["gz"]     = "application/gzip";
			_mimetypes["htm"]    = "text/html";
			_mimetypes["html"]   = "text/html";
			_mimetypes["ico"]    = "image/vnd.microsoft.icon";
			_mimetypes["ics"]    = "text/calendar";
			_mimetypes["jar"]    = "application/java-archive";
			_mimetypes["jpeg"]   = "image/jpeg";
			_mimetypes["jpg"]    = "image/jpeg";
			_mimetypes["js"]     = "text/javascript";
			_mimetypes["json"]   = "application/json";
			_mimetypes["jsonld"] = "application/ld+json";
			_mimetypes["mid"]    = "audio/midi audio/x-midi";
			_mimetypes["midi"]   = "audio/midi audio/x-midi";
			_mimetypes["mjs"]    = "text/javascript";
			_mimetypes["mp3"]    = "audio/mpeg";
			_mimetypes["mp4"]    = "video/mp4";
			_mimetypes["mpeg"]   = "video/mpeg";
			_mimetypes["mpkg"]   = "application/vnd.apple.installer+xml";
			_mimetypes["odp"]    = "application/vnd.oasis.opendocument.presentation";
			_mimetypes["ods"]    = "application/vnd.oasis.opendocument.spreadsheet";
			_mimetypes["odt"]    = "application/vnd.oasis.opendocument.text";
			_mimetypes["oga"]    = "audio/ogg";
			_mimetypes["ogv"]    = "video/ogg";
			_mimetypes["ogx"]    = "application/ogg";
			_mimetypes["opus"]   = "audio/opus";
			_mimetypes["otf"]    = "font/otf";
			_mimetypes["pdf"]    = "application/pdf";
			_mimetypes["php"]    = "application/x-httpd-php";
			_mimetypes["png"]    = "image/png";
			_mimetypes["ppt"]    = "application/vnd.ms-powerpoint";
			_mimetypes["pptx"] =
				"application/"
				"vnd.openxmlformats-officedocument.presentationml.presentation";
			_mimetypes["rar"]   = "application/vnd.rar";
			_mimetypes["rtf"]   = "application/rtf";
			_mimetypes["sh"]    = "application/x-sh";
			_mimetypes["svg"]   = "image/svg+xml";
			_mimetypes["tar"]   = "application/x-tar";
			_mimetypes["tif"]   = "image/tiff";
			_mimetypes["tiff"]  = "image/tiff";
			_mimetypes["ts"]    = "video/mp2t";
			_mimetypes["ttf"]   = "font/ttf";
			_mimetypes["txt"]   = "text/plain";
			_mimetypes["vsd"]   = "application/vnd.visio";
			_mimetypes["wav"]   = "audio/wav";
			_mimetypes["weba"]  = "audio/webm";
			_mimetypes["webm"]  = "video/webm";
			_mimetypes["webp"]  = "image/webp";
			_mimetypes["woff"]  = "font/woff";
			_mimetypes["woff2"] = "font/woff2";
			_mimetypes["xhtml"] = "application/xhtml+xml";
			_mimetypes["xls"]   = "application/vnd.ms-excel";
			_mimetypes["xlsx"] =
				"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
			_mimetypes["xml"] = "application/xml";
			_mimetypes["xul"] = "application/vnd.mozilla.xul+xml";
			_mimetypes["zip"] = "application/zip";
		}
		MimeTypes(MimeTypes const &);      // Don't Implement.
		void operator=(MimeTypes const &); // Don't implement
};
