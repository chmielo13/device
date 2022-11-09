

// const int 

char mystr[] = "Hello World!!!";

char * mystrarray[] = {
    "ONE", "TWO", "THREE"
};


char * my_array_of_string_arrays[][2] {
    { "00", "01" },
    { "10", "11" },
    { "20", "21" }
};


/* 

In case you want to loop through the outer array.
Accepts a larger than necessary size, but does not allow a smaller than
necessary size. 

Note that Ted Zhu finds that it is bad practice to set explicit array size
bigger than necessary, because junk data was found attempting to access
an element at index < BAD_EXPLICIT_ARRAY_SIZE, but with unspecified data.

 */
const int BAD_EXPLICIT_ARRAY_SIZE = 5;
char * my_array_of_string_arrays_explicit[BAD_EXPLICIT_ARRAY_SIZE][2] {
    { "00a", "01a" },
    { "10a", "11a" },
    { "20a", "21d" }
};

const int GOOD_EXPLICIT_ARRAY_SIZE = 3;
char * my_array_of_string_arrays_explicit_good[GOOD_EXPLICIT_ARRAY_SIZE][2] {
    { "00b", "01b" },
    { "10b", "11b" },
    { "20b", "21d" }
};

/* Junk data was also found accessing elements here after index 2. Therefore, this is also bad practice. */
char * mybadexplicit1d[10] = {
    "ONEaaaaaa", "TWObbbbbb", "THREEcccccc"
};

char * mygoodexplicit1d[3] = {
    "ONEaaaaaa", "TWObbbbbb", "THREEcccccc"
};


void printNBytes(char * location, int num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        char byte = location[i];
        // Serial.println(byte);
        Serial.println(int(byte));
    }
}

void setup() {

    Serial.begin(9600);

    // Wait until Serial port is open.
    while(!Serial);

    Serial.println("Hello World from String Char test!");
    Serial.println("Hello World from String Char test2!");
    Serial.println(mystr);
    Serial.println(mystrarray[0]);

    Serial.println(my_array_of_string_arrays[2][1]);
    Serial.println(my_array_of_string_arrays_explicit[2][1]);
    Serial.println(my_array_of_string_arrays_explicit[3][1]);
    Serial.println(strlen(my_array_of_string_arrays_explicit[3][0]));

    Serial.println(mybadexplicit1d[2]);
    Serial.println(mybadexplicit1d[4]);

    Serial.println(strlen(mybadexplicit1d[2]));
    Serial.println(strlen(mybadexplicit1d[4]));
    
    char hello[] = "Hello World";
    // printNBytes(hello, 13);
    Serial.println(hello);
    printNBytes(mybadexplicit1d[4], 13);


}



void loop() {
}

