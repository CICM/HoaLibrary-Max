inlets  = 1;
outlets = 1;

var d;

function init()
{
	d = new Dict;
	d.import_json("hoa.objectlist.json");
}
init();

function anything()
{
	var args = arrayfromargs(messagename, arguments);

	if (messagename == "getkeys") 
	{
		outlet(0, d.getkeys());
	}
	if (messagename == "objectlist") 
	{
		if(d.get(arguments[0]))
		{
			var objects = d.get(arguments[0] + "::objects");
			outlet(0, objects);
		}
		else
			outlet(0, "---");
	}
	else if (messagename == "get_category_description_from_category_index") 
	{
		var index = arguments[0];
		var keys = d.getkeys();
		if (keys[index]) 
		{
			var desc = d.get(keys[index] + "::description");
			if (desc)
				outlet(0, "set", desc);
			else
				outlet(0, "set");
		}
		else
			outlet(0, "set");
	}
	else if (messagename == "get_objectname_from_category_and_index") 
	{
		var category = arguments[0];
		var index = arguments[1];
		var objects = d.get(arguments[0] + "::objects");
		var object = objects[index];
		outlet(0, object);
	}
	else if (messagename == "get_objectref_from_category_and_index") 
	{
		var category = arguments[0];
		var index = arguments[1];

		if (index < 0) 
		{
			outlet(0, "set");
			return;
		};
		
		if(d.get(category))
		{
			var objects = d.get(category + "::objects");
			var objectName = objects[index];

			var dict = max.getrefdict(objectName);

			if (typeof(dict) == "object") 
			{
				//outlet(0, dict.get("digest"));
				outlet(0, "set", dict.get("description"));
				dict.freepeer();
			}
			else
			{
				outlet(0, "set");
				return;
			};
		}
		else
			outlet(0, "set");
	};
}