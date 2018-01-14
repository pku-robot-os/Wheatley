#ifdef __SERVER_H__ 
#define __SERVER_H__
struct server_node{
	char* path;
	wchar_t* pattern;
};
wchar_t server_exec(wchar_t input);
wchar_t server_ins(struct server_node); 
struct server_node search(wchar_t input );
wchar_t server_del(int id);
#endif
