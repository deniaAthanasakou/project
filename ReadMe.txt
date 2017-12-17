Ομάδα:
Αντωνία Αθανασάκου 	1115201400004
Στεφανία Πάτσου 	1115201400156

Εκτέλεση
Στο πρόγραμμα περιέχεται έτοιμο το Makefile , οπότε η εκτέλεση του αρχείου είναι make -> ./ngrams -i init_file -q query_file 

PART 1

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

PART 2

Έπειτα από συμβουλές και απαιτήσεις για αλλαγή δομών και συναρτήσεων έχουμε τα εξής:

Δομές 

Αλλαγμένες δομές:
struct dataNode{
	arrayOfStructs* nextWordArray;	//pointer se array apo dataNodes (επόμενη λέξη)
	char* word;						//leksh
	bool isFinal;					//final or not
	
	//added
	bool isDynamic;					//if true strlen(word)>20
	char* dynamicWord;				//at first is null
	int noOfChars;
	signed short* staticArray;		//for static files
	int staticArrayLength;			//for static files
};

struct checkItemExists{
	int position;	//position of element to insert
	bool exists;	// if exists true
	
	//added
	dataNode* insertedNode;		//address of found node
};

Προστιθέμενες δομές από τα καινούρια αρχεία

Δομή του Linear HashTable.
struct HashTable{
	int length;					//m
	int initialLength; 
	Bucket* buckets;			//array of buckets
	int level;					//level of hashing
	int bucketToBeSplit;		//p
	int numberOfBuckets;		//real length of bukets
};

Δομή του bucket.
struct Bucket{
	int length;					
	dataNode* cells;			//array of struct datanode
	int position;				//position of last element
	int noOfCells;				//c initial length
	bool overflowed;
};

Δομή topK η οποία περιέχει το ngram μαζί με τις φορές που το έχουμε συναντήσει σε query.
typedef struct topKStruct{
	char* ngram;
	int occurences;
}topKStruct;

Δομή η οποία περιέχει έναν πίνακα από topKStructs, την κενή θέση για εισαγωγή και το συνολικό length.
typedef struct topKArray{
	int length;
	topKStruct *array;
	int positionInsertion;		//empty position for insert
}topKArray;

Δομή του BloomFilter. Αποτελείται από έναν πίνακα από bool με συγκεκριμένο size(HASH_SIZE) και έναν αριθμό από hashes.
typedef struct BloomFilter{
	bool bitVector[HASH_SIZE];
	uint8_t numHashes;
}BloomFilter;


Main
Η main.c αποτελείται από 2 λειτουργίες μετά από διόρθωση. Αρχικά, ελέγχουμε για σωστά ορίσματα. 
Στην συνέχεια καλούμε την initialize η οποία εισάγει τα στοιχεία κάθε γραμμής στην δομή μας. Σε αυτό το σημείο ελέγχουμε κι αν είναι
STATIC ή DYNAMIC και ανάλογα καλούμε την recreateStructure. Και τέλος, την executeQueryFile η 
οποία εκτελεί το query αρχείο, κάνοντας είτε εισαγωγή είτε αναζήτηση είτε διαγραφή στην δομή μας ανάλογα με το πρώτο γράμμα.

Αρχεία 
Πέρα από την main.c , έχουμε και τα εξής αρχεία στα οποία έχουν προστεθεί και επιπλέον για τις ανάγκες της άσκησης:
	-stack.c : Περιέχει τις μεθόδους για την διαχείριση μίας στοίβας, όπως αρχικοποίηση (initializeStack), διπλασιασμός (doubleLengthOfStack),
			   διαγραφή (deleteStack), διαγραφή ενός στοιχείου (pop), εισαγωγή (push), απεικόνιση (displayStack), έλεγχος για το αν είναι κενή (isEmpty). 
	-struct.c : Περιέχει τις μεθόδους για την διαχείριση της ολικής δομής μας, όπως αρχικοποίηση (initializeArray), διπλασιασμός (doubleLength),
			    διαγραφή (deleteArray) ,διαγραφή ενός στοιχείου (deleteDataNode), 
				//added
				αρχικοποίηση dataNode (initializeDataNode), εκτύπωση πίνακα staticArray (printStaticArray), διαγραφή επιπέδου μίας ολόκληρης φράσης (deleteArray1Layer).
	-cutest-1.5 : Περιέχει τα unit testing για την κάθε συνάρτηση με framework το CuTest.
	-auxMethods.c : Περιέχει τις μεθόδους που μας βοηθούν να υλοποιήσουμε τις μεθόδους στο func.c, όπως binarySearch , insertionSort κτλπ.
	-func.c : Περιέχει τις κύριες μεθόδους για την υλοποίηση μας δηλαδή τις: εισαγωγή ngram ενός ngram(insert_ngram) , αναζήτηση ενός ngram (search_ngram),
			  διαγραφή ενός ngram (delete_ngram),
			  //added
			   αναζήτηση ngram μετά από compress(search_ngram_StaticVersion).
	//added
	-bloomfilter.c : αρχικοποίηση filter (initializeFilter), επιστροφή hashes από Murmur (getHashesMurmur), υπολογισμός κ-οστής hash (kthHash),
					 εισαγωγή στο filter (addFilter), έλεγχος ύπαρξης ngram (possiblyContains), διαγραφή filter (freeFilter).
	-hashTable.c : δημιουργία γραμμικού hash (createLinearHash), καταστροφή γραμμικού hash (destroyLinearHash), 
	               εισαγωγή στο γραμμικό hash (insertTrieNode_insertTrieNodeAgain), αναζήτηση κόμβου σε bucket (lookupTrieNode),
				   αρχικοποίηση bucket (initializeBucket), επιστροφή σωστού bucket από hashing (getBucketFromHash) , αναδιανομή κόμβων σε buckets (splitBucket), 
				   αύξηση level (levelUp), διαγραφή ενός bucket και όλων των nextWordArray του (deletionSortBucket),
				   εκτύπωση ενός bucket (printBucket), εκτύπωση όλου του hashTable (printBuckets), αύξηση αριθμού κελιών σε ένα bucket (createOverflowCells),
				   επιστροφή συγκεκριμένου κελιού (getCell), διαγραφή ενός bucket χωρίς την διαγραφή όλων των nextWordArray του (deletionSortWithoutErasingNextArray).
	-topK.c : αρχικοποίηση topKArray (initializeTopKArray), διπλασιασμός topKArray (doubleTopKArray), αρχικοποίηση topKStruct (initializeTopKStruct),
			  εισαγωγή στο topKArray (insertTopArray), εκτύπωση δοσμένων topK από το topKArray (printTopK), εκτύπωση όλων των topK από το topKArray (printFullArrayTop),
			  καταστροφή topKArray (destroyTopArray), ταξινόμηση topKArray με HeapSort (HeapSort_BuildHeap), ταξινόμηση ως προς τα strings (HeapifyStrings), 
			  ταξινόμηση ως προς τα occurences (HeapifyIntegers), αύξηση του occurences ενός topKStruct αν το string υπάρχει (binarySearchTopK).
	-compress.c : ανακατασκευή δομής συμπιέζοντας τους κόμβους (recreateStructure), συμπίεση δύο κόμβων (compress), 
				  αναδρομική ανακατασκευή δομής σε κάθε arrayOfStruct (recursiveCompression), έλεγχος συμπίεσης και ύστερα συμπίεση (checkForCompression), 
				  επιστροφή ενός array of strings ανάλογα με τα πόσα αποτελείται ένας κόμβος (getNgramFromNode) .
	
Unit testing
Για το unit testing χρησιμοποιήσαμε το framework CuTest. Καθεμία από εμάς, υλοποίησε έναν αριθμό από Test Functions για το κάθε αρχείο.
Test units από το 1ο μέρος της άσκησης έχουν τροποποιηθεί ανάλογα με το framework. Πολλά από αυτά δεν έχουν αλλάξει ως προς τις περιπτώσεις ελέγχου.
Τα αρχεία αυτά βρίσκονται στον φάκελο cutest-1.5 με ξεχωριστό Makefile από αυτό της λειτουργίας της άσκησης.

Σχόλια
-Αν το topK > topArray->positionInsertion (ουσιαστικά δίνει αριθμό με παραπάνω topK από ότι έχει ο πίνακας), αναθέτουμε στο topK το topArray->positionInsertion.
-Το medium dataset δεν δουλεύει στο initialization.
-Ως hashing στο bloomfilter χρησιμοποιήσαμε την MurmurHash3_x64_128. Η συγκεκριμένη διαιρεί το κλειδί σε chunks και μέσω rotation επιστρέφει ένα hash 128bit. 
 ιδανικό για 64-bit μηχάνημα.
-Χρόνοι σε κάθε dataset:
	small_dynamic: 1.03 min
	small_static πριν το compress: 40sec
	small_static μετά το compress: 51sec






