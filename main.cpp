/*
 * The Tree Command
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 3
 */

#include <infos.h>

const char* concat_char_arr(const char* lhs, const char* rhs){
    // concatenates two strings
    
    int lhs_size = strlen(lhs);
    int rhs_size = strlen(rhs);

    // printf("this word %s has size %d \n", lhs, lhs_size);
    // printf("this word %s has size %d \n", rhs, rhs_size);

    int new_size = lhs_size + rhs_size;

    char concat[new_size+1];
    concat[new_size+1] = 0;
    // char* concat = new char[new_size];
    // _data = new char[_size + 1];
    

    // printf("the new size is %d \n", new_size);

    // need to add the slash at the relevant location
    
   

    for (unsigned int i=0; i < lhs_size; i++) {
        // printf("%d\n", i);
        concat[i] = lhs[i];
    }
    concat[lhs_size] = *"/";

    printf("\n");

    for (unsigned int i=0; i < rhs_size; i++) { 
        // printf("%d\n", lhs_size+i);
        concat[lhs_size+i+1] = rhs[i];
    }


    // printf("\n");
    const char* i = concat;
    printf("concatenation: %s\n", concat);
    return i;
}



int recurse_dir(const char* dir_name, const char* prefix, int recursion_level) {
    // printf("look this is the length (%s) \n", char_arr_size(dir_name));
    // const char* test = concat_char_arr("hello", "world");
    // printf("testing\n");
    // printf("aughhhhhhh %s", test);
    // printf("prefix length (%d) ", strlen(prefix));
    // if (prefix) {
    //     printf("helloooo");
    // } else{
    //     printf("hone");
    // }
    // printf("%d", sizeof(dir_name));
    // if (prefix) {dir_name = prefix+dir_name;}

    if (prefix) {
        dir_name = concat_char_arr(prefix, dir_name);
        // printf("concatenation importante: %s\n", dir_name);
    }

    printf("%s \n", dir_name);
    
    HDIR dir = opendir(dir_name, 0);
  
    if (is_error(dir)){
        printf("this right here is an error I'm throwing cuz you tried to open %s ", dir);
        return 1;
    }
    printf("directory opened");
    struct dirent de;

    while (readdir(dir, &de)) {
        printf("%s (%u bytes) \n", de.name, de.size);
        // int test = recurse_dir(de.name, dir_name, 1);
    }


    closedir(dir);

    return 0;
}

void print_dir(int recursion_level) {
    printf("pass");
}


int main(const char *cmdline)
{
    // TODO: Implement me!
    const char* input = cmdline;


    // default is no input directory, which just prints the default location (/usr)
    if (!input || strlen(input) == 0) {
        printf("case: default \n");
        // printf("directory: /usr \n");
        input = "/usr";
    } else {
        printf("non-null, we have - %s", input);
    }

    // now to open the input directory

    // recurse_dir(input, NULL, 0);


    HDIR dir = opendir(input, 0);
  
    
    struct dirent de;

    while (readdir(dir, &de)) {
        // printf("%s (%u bytes) \n", de.name, de.size);
        const char* new_name = concat_char_arr(input, de.name);
        HDIR dir2 = opendir(new_name, 0);
        struct dirent de2;
        printf("the new name is %s\n", new_name);
        while (readdir(dir2, &de2)) {
        
            printf("=-=-= %s (%u bytes) \n", de2.name, de2.size);
        }
    }





    return 0;
}


// pasted from ls
// {
	
// 	HDIR dir = opendir(path, 0);
// 	if (is_error(dir)) {
// 		printf("Unable to open directory '%s' for reading.\n", path);
// 		return 1;
// 	}
	
// 	printf("Directory Listing of '%s':\n", path);

// 	struct dirent de;
// 	while (readdir(dir, &de)) {
// 		printf("  %s (%u bytes)\n", de.name, de.size);
// 	}
// 	closedir(dir);

// 	return 0;
// }
