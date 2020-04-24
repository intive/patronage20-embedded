static String report_hsv_color(int hue, int saturation, int value, int id, String type)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = type;
    root["hue"] = hue;
    root["saturation"] = saturation;
    root["value"] = value;
    serializeJson(root, json_output);
    return json_output;
}
