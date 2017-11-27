Ομάδα:
Αντωνία Αθανασάκου 	1115201400004
Στεφανία Πάτσου 	1115201400156


Εκτέλεση
Στο πρόγραμμα περιέχεται έτοιμο το Makefile , οπότε η εκτέλεση του αρχείου είναι make -> ./ngram -i init_file -q query_file 


Δομές

Η ολική δομή της άσκησης αποτελείται από τα εξής:

Το node κάθε θέσης στο arrayOfStructs. Αποτελεί struct για την πληροφορία της κάθε λέξης.
struct dataNode{
	arrayOfStructs* nextWordArray;	//pointer se array apo dataNodes (επόμενη λέξη)
	char* word;						//leksh
	bool isFinal;					//final or not
};

Struct που περιέχει το μέγεθος , ποιά είναι η επόμενη θέση για εισαγωγή και ο πίνακας με τα dataNodes.
Αρχικοποιείται από την main.c και αποτελεί την κύρια δομή του προγράμματος, αφού το χρησιμοποιούμε για εισαγωγή,
αναζήτηση ή διαγραφή κάποιου ngram.
struct arrayOfStructs{
	int length;				//diastash array initialized 10
	dataNode* array;		//pointer se array apo structs dataNodes
	int position;			//the position of the first null element of array
};

Η στοίβα που μέσω αυτής βλέπουμε σε κάθε επίπεδο ποιό word πρέπει να διαγράψουμε.Χρησιμοποιείται στην delete_ngram.
struct stack{
	int* positionsToDelete;		//pinakas apo theseis pou prepei na diagrafoun
	int top;					//thesh tou teleutaiou stoixeiou;
	int length;					//megethos tou pinaka
};

Βοηθητική δομή που επιστρέφει την θέση που πρέπει να εισαχθεί ένα στοιχείο και αν υπήρχε πριν την εισαγωγή.
Χρησιμοποιείται σε binarySearch και insertionSort μεθόδους.
struct checkItemExists{
	int position;	//position of element to insert
	bool exists;	// if exists true
};

Βοηθητική δομή που χρησιμοποιούμε για να μετατρέψουμε μία γραμμή σε πίνακα από Strings, αλλά και για να ξέρουμε πόσες 
λέξεις περιέχονται σε μία γραμμή.
typedef struct arrayWords{
	int length;					//length of array of strings
	char** words;			//array of strings
}arrayWords;


Main
Η main.c αποτελείται από 3 λειτουργίες. Αρχικά, ελέγχουμε για σωστά ορίσματα. Έπειτα, εκτελούμε το unit testing. 
Στην συνέχεια καλούμε την initialize η οποία εισάγει τα στοιχεία κάθε γραμμής στην δομή μας. Και τέλος, την executeQueryFile η 
οποία εκτελεί το query αρχείο, κάνοντας είτε εισαγωγή είτε αναζήτηση είτε διαγραφή στην δομή μας ανάλογα με το πρώτο γράμμα.


Αρχεία 
Πέρα από την main.c , έχουμε και τα εξής :
	-stack.c : Περιέχει τις μεθόδους για την διαχείριση μίας στοίβας, όπως αρχικοποίηση (initializeStack), διπλασιασμός (doubleLengthOfStack),
			   διαγραφή (deleteStack), διαγραφή ενός στοιχείου (pop), εισαγωγή (push), απεικόνιση (displayStack), έλεγχος για το αν είναι κενή (isEmpty). 
	-struct.c : Περιέχει τις μεθόδους για την διαχείριση της ολικής δομής μας, όπως αρχικοποίηση (initializeArray), διπλασιασμός (doubleLength),
			    διαγραφή (deleteArray) ,διαγραφή ενός στοιχείου (deleteDataNode).
	-test.c : Περιέχει τα unit testing για την κάθε συνάρτηση.
	-auxMethods.c : Περιέχει τις μεθόδους που μας βοηθούν να υλοποιήσουμε τις μεθόδους στο func.c, όπως binarySearch , insertionSort κτλπ.
	-func.c : Περιέχει τις κύριες μεθόδους για την υλοποίηση μας δηλαδή τις: εισαγωγή ngram ενός ngram(insert_ngram) , αναζήτηση ενός ngram (search_ngram),
			  διαγραφή ενός ngram (delete_ngram).


Διαχωρισμός unit testing

Η Στεφανία Πάτσου υλοποίησε τα εξής:

-void test_initializeArray();
-void test_doubleLength();
-void test_deleteArray();
-void test_deleteDataNode();
-void test_insert_ngram();
-void test_search_ngram();
-void test_binarySearch();
-void test_insertionSort();
-void test_callBasicFuncs();

Η Αντωνία Αθανασάκου υλοποίησε τα εξής:

-void testAllFunctions();
-void test_initializeStack();
-void test_doubleLengthOfStack();
-void test_push();
-void test_isEmpty();
-void test_pop();
-void test_deleteStack();
-void test_delete_ngram();
-void test_checkIfStringExists();
-void test_deleteArrayOfWords();
-void test_stringToArray();
-void test_initialize();
-void test_deletionSort();
-void test_executeQueryFile();


Σχόλια

-Η περισσότερη υλοποίηση έγινε με την συνεχή συνεργασία μας για όλες τις μεθόδους.
-Στην γραμμή 33 στο αρχείο test.work , για κάποιο λόγο αν υπήρχε το 'r' στην λέξη 'cancer', το πρόγραμμα θεωρούσε ότι έβρισκε EOF. Οπότε, σβήσαμε το τελευταίο γράμμα. Ο φάκελος με τα αρχεία test περιλαμβάνεται στο φάκελό μας.
-Δημιουργήσαμε έναν φάκελο testingFiles ο οποίος έχει αρχεία δοκιμών για το unit testing, καθώς και αρχεία που χρησιμοποιήσαμε για την εκτέλεση και τον έλεγχο των αποτελεσμάτων του προγράμματός μας.

