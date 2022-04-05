/*
 * The Tree Command
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 3
 */


#include <infos.h>


// global variables
// true if regex flag has been activated
bool regex = false;
// stores regex pattern to be matched
const char* regex_pattern;


/**
 * regex match function returns true if inp matches regex, else false
 * @param inp - check if inp matches regex pattern
*/
bool regex_match(const char* inp) {
	// if regex_pattern matches inp
	return true;
	// else
	// return false
}


/**
 * print function prints directory recursively according to levels
 * if regex mode is activated, only prints if matches regex
 * @param name - name of directory/item to be printed
 * @param level - recursion level
 */ 
void print(const char* name, int level) {
	// if regex mode is activated
	if (regex) {
		// if regex does not match name, do not print
		if (!regex_match(name)){
			return;
		}
	}

	// else begin printing
	int lvl = level;
	
	// for each level of recursion print an indent
	while (lvl != 0) {
		printf("|   ");
		lvl--;
	}

	// print the name prefix
	printf("|--- ");

	// print the name of the dir or file
	printf("%s\n", name);
}

/**
 * recurse provided directories and print contents
 * @param dir_name - name of directory to recurse
 * @param level - recursion level (starting from zero, incrementing on each recurse_dir call)
 */
void recurse_dir(const char* dir_name, int level) {

	// open dir
	HDIR dir = opendir(dir_name, 0);
	// struct to hold directory items
	struct dirent de;
	// initiate item as not a directory
  	bool is_dir = false;
  	while (readdir(dir, &de)) {
		  
		// cocatentate directory name with a slash and the directory item name
		// result is dir_name/de.name

		const char *a = dir_name;
		const char *b = de.name;

		// buffer to hold new name
		char buffer[strlen(a) + strlen(b)];

		// iterate over dir_name, adding to buffer
		int i = 0;
		while (*a && *a != ' ') {
		  	buffer[i++] = *a++;
	    }

		// add slash after dir_name
	    buffer[i] = *"/";
        i++;

	    if (*a) a++;

		// iterate over de.name, adding to buffer
	    while (*b && *b != ' ') {
		   	buffer[i++] = *b++;
	    }

		// add escape character to buffer
	    buffer[i] = 0;

	    if (*b) b++;

		// assign buffer to new_dir
      	const char* new_dir = buffer;

		// print directory item
		print(de.name, level);
		
		// open the new directory
        HDIR dir2 = opendir(new_dir, 0);

		// if there is not an error opening the directory, the new item is a directory
        if (!is_error(dir2)){
            is_dir = true;
        } 

		// thus recurse this direcory
        if (is_dir) {
            recurse_dir(new_dir, level+1);
        }
		closedir(dir2);

		// set is_dir to false for the next entry
        is_dir = false;
    }
	closedir(dir);


}


/**
 * main function takes initial call with cmdline arguments
 * parses these arguments and determines wether or not the regex flag is used
 * @param cmdline - the command line arguments
 */
int main(const char *cmdline) {

    const char* input = cmdline;

    // if no arguments are given
    if (!input || strlen(input) == 0) {
		// recurse the /usr director
        input = "/usr";
		recurse_dir(input, 0);
    } else {
        // otherwise check for regex and obtain the provided directory

		// dir end pos is the location after which the director location ends
		int dir_end_pos = 0;

		// loop over the input
		while (*input) {
			
			

			// upon reaching a space, another argument is provided
			if (*input++ == *" "){

				// if the -P regex flag is given
				if (*input++ == *"-" && *input++ == *"P"){

					// increment input to move past space to regex pattern
					input++;

					// set regex to true and break as the regex flag is given
					regex=true;
					break;
				}
			}

			// increment input while director is still being given
			dir_end_pos++;
		}
		// as input has been incremented past the directory and -P flag,
		// what remains is the regex pattern. thus input := regex patter

		// create a buffer starting at 0 and ending where the directory input ends
		char buffer[dir_end_pos];
		// get cmd line arguments again
		const char* parse_dir = cmdline;

		// loop over the arguments, adding the directory location to buffer
		for (unsigned int i=0; i<dir_end_pos; i++) {
			buffer[i] = *parse_dir++;
		}
		// add terminating character
		buffer[dir_end_pos] = 0;

		// set parse_dir to the buffer
		// parse_dir now contains the input directory
		parse_dir = buffer;
				
		// if regex was provided, set the global pattern to the provided pattern
		if (regex){
			regex_pattern = input;
			printf("given regex pattern: %s\n", regex_pattern);
		}

		// if the provided directory to recurse is invalid, return 1 and exit
		HDIR dir = opendir(parse_dir, 0);
		if (is_error(dir)) {
			printf("%s is not a valid directory\n", parse_dir);
			closedir(dir);
			return 1;
		}
		closedir(dir);

		// otherwise, regardless of regex, recurse the directory given in parse_dir
		recurse_dir(parse_dir, 0);
    }

	// if this point is reached, recurse_dir was successful 
	// return 0 indicating success
    return 0;
}