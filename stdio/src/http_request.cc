/**
 * Copyright (c) 2012 Martin M Reed
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "http_request.h"

CURL* hbc::http_request::get_curl()
{
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

	if (method.compare(HTTP_POST) == 0) {
	    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
	}
	else if (method.compare(HTTP_GET) == 0) {
	    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	}

    struct curl_slist* headers = NULL;

    std::map<std::string, std::string>::iterator i;
	for (i = this->headers.begin(); i != this->headers.end(); i++)
	{
		std::string header;
		header.append(i->first);
		header.append(": ");
		header.append(i->second);
	    headers = curl_slist_append(headers, header.c_str());
	}

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    return curl;
}
