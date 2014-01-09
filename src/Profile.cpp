/**
 * @file
 * @brief Source file for Profile class
 * @author Jonathan Thomas <jonathan@openshot.org>
 *
 * @section LICENSE
 *
 * Copyright (c) 2008-2013 OpenShot Studios, LLC
 * (http://www.openshotstudios.com). This file is part of
 * OpenShot Library (http://www.openshot.org), an open-source project
 * dedicated to delivering high quality video editing and animation solutions
 * to the world.
 *
 * OpenShot Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenShot Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenShot Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/Profile.h"

using namespace openshot;


// @brief Constructor for Profile.
// @param path 	The folder path / location of a profile file
Profile::Profile(string path) throw(InvalidFile, InvalidJSON) {
	try
	{
		// Read the profile file
		ifstream myfile (path.c_str());
		if (myfile.is_open())
		{
			// Loop through each line
			while (myfile.good())
			{
				// read current line of file
				string line = "";
				getline (myfile, line);

				if (line.length() <= 0)
					continue;

				// Split current line
				QString qline(line.c_str());
				QStringList parts = qline.split( "=" );
				string setting = parts[0].toStdString();
				string value = parts[1].toStdString();
				int value_int = 0;

				// update struct (based on line number)
				if (setting == "description")
					info.description = value;
				else if (setting == "frame_rate_num") {
					stringstream(value) >> value_int;
					info.fps.num = value_int;
				}
				else if (setting == "frame_rate_den") {
					stringstream(value) >> value_int;
					info.fps.den = value_int;
				}
				else if (setting == "width") {
					stringstream(value) >> value_int;
					info.width = value_int;
				}
				else if (setting == "height") {
					stringstream(value) >> value_int;
					info.height = value_int;
				}
				else if (setting == "progressive") {
					stringstream(value) >> value_int;
					info.interlaced_frame = !(bool)value_int;
				}
				else if (setting == "sample_aspect_num") {
					stringstream(value) >> value_int;
					info.pixel_ratio.num = value_int;
				}
				else if (setting == "sample_aspect_den") {
					stringstream(value) >> value_int;
					info.pixel_ratio.den = value_int;
				}
				else if (setting == "display_aspect_num") {
					stringstream(value) >> value_int;
					info.display_ratio.num = value_int;
				}
				else if (setting == "display_aspect_den") {
					stringstream(value) >> value_int;
					info.display_ratio.den = value_int;
				}
				else if (setting == "colorspace") {
					stringstream(value) >> value_int;
					info.pixel_format = value_int;
				}
			}
			myfile.close();
		}


	}
	catch (exception e)
	{
		// Error parsing profile file
		throw InvalidFile("Profile could not be found or loaded (or is invalid).", path);
	}
}

// Generate JSON string of this object
string Profile::Json() {

	// Return formatted string
	return JsonValue().toStyledString();
}

// Generate Json::JsonValue for this object
Json::Value Profile::JsonValue() {

	// Create root json object
	Json::Value root;
	root["height"] = info.height;
	root["width"] = info.width;
	root["pixel_format"] = info.pixel_format;
	root["fps"] = Json::Value(Json::objectValue);
	root["fps"]["num"] = info.fps.num;
	root["fps"]["den"] = info.fps.den;
	root["pixel_ratio"] = Json::Value(Json::objectValue);
	root["pixel_ratio"]["num"] = info.pixel_ratio.num;
	root["pixel_ratio"]["den"] = info.pixel_ratio.den;
	root["display_ratio"] = Json::Value(Json::objectValue);
	root["display_ratio"]["num"] = info.display_ratio.num;
	root["display_ratio"]["den"] = info.display_ratio.den;
	root["interlaced_frame"] = info.interlaced_frame;

	// return JsonValue
	return root;
}

// Load JSON string into this object
void Profile::SetJson(string value) throw(InvalidJSON) {

	// Parse JSON string into JSON objects
	Json::Value root;
	Json::Reader reader;
	bool success = reader.parse( value, root );
	if (!success)
		// Raise exception
		throw InvalidJSON("JSON could not be parsed (or is invalid)", "");

	try
	{
		// Set all values that match
		SetJsonValue(root);
	}
	catch (exception e)
	{
		// Error parsing JSON (or missing keys)
		throw InvalidJSON("JSON is invalid (missing keys or invalid data types)", "");
	}
}

// Load Json::JsonValue into this object
void Profile::SetJsonValue(Json::Value root) {

	if (!root["height"].isNull())
		info.height = root["height"].asInt();
	if (!root["width"].isNull())
		info.width = root["width"].asInt();
	if (!root["pixel_format"].isNull())
		info.pixel_format = root["pixel_format"].asInt();
	if (!root["fps"].isNull()) {
		info.fps.num = root["fps"]["num"].asInt();
		info.fps.den = root["fps"]["den"].asInt();
	}
	if (!root["pixel_ratio"].isNull()) {
		info.pixel_ratio.num = root["pixel_ratio"]["num"].asInt();
		info.pixel_ratio.den = root["pixel_ratio"]["den"].asInt();
	}
	if (!root["display_ratio"].isNull()) {
		info.display_ratio.num = root["display_ratio"]["num"].asInt();
		info.display_ratio.den = root["display_ratio"]["den"].asInt();
	}
	if (!root["interlaced_frame"].isNull())
		info.interlaced_frame = root["interlaced_frame"].asBool();

}
