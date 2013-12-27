#include <jansson.h>
#include <string.h>

void encode_twice()
{
    /* Encode an empty object/array, add an item, encode again */

    json_t *json;
    char *result;

    json = json_object();
    result = json_dumps(json, 0);
    if(!result || strcmp(result, "{}"))
      printf("json_dumps failed");
    free(result);

    json_object_set_new(json, "foo", json_integer(5));
    result = json_dumps(json, 0);
    if(!result || strcmp(result, "{\"foo\": 5}"))
      printf("json_dumps failed");
   	
   	printf("encode_twice() result1 = %s\n", result); 
   
    free(result);

    json_decref(json);

    json = json_array();
    result = json_dumps(json, 0);
    if(!result || strcmp(result, "[]"))
      printf("json_dumps failed");
    
    printf("encode_twice() result2 = %s\n", result); 
    
    free(result);

    json_array_append_new(json, json_integer(5));
    json_array_append_new(json, json_integer(10));

    result = json_dumps(json, 0);
    if(!result || strcmp(result, "[5]"))
      printf("json_dumps failed");
      
    printf("encode_twice() result3 = %s\n", result); 
      
    free(result);

    json_decref(json);
}

void circular_references()
{
    /* Construct a JSON object/array with a circular reference:

       object: {"a": {"b": {"c": <circular reference to $.a>}}}
       array: [[[<circular reference to the $[0] array>]]]

       Encode it, remove the circular reference and encode again.
    */

    json_t *json, *jsonArray;
    char *result;

    json = json_object();

	/*
	jsonArray = json_array(); 
    
    json_array_append_new(jsonArray, json_integer(5));
    json_array_append_new(jsonArray, json_integer(10));
    
    json_object_set_new(json, "a", json_string("aaa"));
    json_object_set_new(json, "b", json_object());
    json_object_set(json_object_get(json, "b"), "c", jsonArray);
    //json_object_set_new(json_object_get(json, "a"), "b", json_object());
//    json_object_set(json_object_get(json_object_get(json, "a"), "b"), "c",
//                    json_object_get(json, "a"));

//		json_array_append_new(jsonArray, json_integer(5));
//    json_array_append_new(jsonArray, json_integer(10));
//	 	json_object_set(json_object_get(json_object_get(json, "a"), "b"), "c",
//                     jsonArray);
   
   	                

    //if(json_dumps(json, 0))
    //    printf("json_dumps encoded a circular reference!");

    //json_object_del(json_object_get(json_object_get(json, "a"), "b"), "c");

    result = json_dumps(json, 0);
//    if(!result || strcmp(result, "{\"a\": {\"b\": {}}}"))
//        printf("json_dumps failed!");
    
    printf("circular_references() result1 = %s\n", result); 
    
    free(result);

		json_decref(jsonArray);
    json_decref(json);

*/


    json = json_array();
    json_array_append_new(json, json_array());
    json_array_append_new(json_array_get(json, 0), json_array());
    json_array_append(json_array_get(json_array_get(json, 0), 0),
                      json_array_get(json, 0));

		result = json_dumps(json, 0);
    if(result)
        printf("json_dumps encoded a circular reference!");

		printf("circular_references() result2 = %s\n", result); 
		
    json_array_remove(json_array_get(json_array_get(json, 0), 0), 0);

    result = json_dumps(json, 0);
    if(!result || strcmp(result, "[[[]]]"))
        printf("json_dumps failed!");
    
    printf("circular_references() result3 = %s\n", result); 
    
    free(result);

    json_decref(json);
}

void encode_other_than_array_or_object()
{
    /* Encoding anything other than array or object should only
     * succeed if the JSON_ENCODE_ANY flag is used */

    json_t *json;
    FILE *fp = NULL;
    char *result;

    json = json_string("foo");
    if(json_dumps(json, 0) != NULL)
        printf("json_dumps encoded a string!");
    if(json_dumpf(json, fp, 0) == 0)
        printf("json_dumpf encoded a string!");

    result = json_dumps(json, JSON_ENCODE_ANY);
    if(!result || strcmp(result, "\"foo\"") != 0)
        printf("json_dumps failed to encode a string with JSON_ENCODE_ANY");

		printf("encode_other_than_array_or_object() result1 = %s\n", result); 
		
    free(result);
    json_decref(json);

    json = json_integer(42);
    if(json_dumps(json, 0) != NULL)
        printf("json_dumps encoded an integer!");
    if(json_dumpf(json, fp, 0) == 0)
        printf("json_dumpf encoded an integer!");

    result = json_dumps(json, JSON_ENCODE_ANY);
    if(!result || strcmp(result, "42") != 0)
        printf("json_dumps failed to encode an integer with JSON_ENCODE_ANY");

		printf("encode_other_than_array_or_object() result2 = %s\n", result);
		
    free(result);
    json_decref(json);


}

int main()
{
    encode_twice();
    circular_references();
    encode_other_than_array_or_object();
    return 0;
}
