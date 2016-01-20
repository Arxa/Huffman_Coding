#include "MyStructs.c"
#include "Huffman_Coding.h"

//------------------------------//
int main(int argc, char *argv[])
{
    if (getAnswer()==1)
    {
        textMode = false;
        read_ColourTableValues_File();
    }
    else
    {
        textMode = true;
        read_TextMessage_File();
    }
    findAllValuesFrequency(FHead);
    createTree();
    getch();
    return 0;
}
//------------------------------//


int getAnswer()
{
    int input = 0;
    char answer[1];
    bool repeat;
    printf("\nThis program encodes and compresses data, using the Huffman's Coding technique.\n");
    do
    {
        printf("\nPress 1: To encode Colour Table Values\nPress 2: To encode a Text Message\n\n");
        scanf("%s",answer);
        if (strcmp(answer,"1")==0)
        {
            input = 1;
            repeat = false;
        }
        else if (strcmp(answer,"2")==0)
        {
            input = 2;
            repeat = false;
        }
        else
        {
            repeat = true;
            printf("\nPlease enter a valid answer\n");
        }
    }while(repeat);
    printInstructions(input);
    return input;
}

void printInstructions(int input)
{
    char answer[1];
    if (input==1)
    {
        printf("\nPlease open the 'Colour_Table_Values.txt' file, \nand save your Colour Table Values"
                       " seperated by space.\nTake notice that a Colour Table Value must be between 0-255.\n"
                       "Press 1 once you are done, or any key to exit this program.\n");
        scanf("%s",answer);
        if (strcmp(answer,"1")!=0)
        {
            exit(0);
        }
    }
    else
    {
        printf("\nPlease open the 'Text_Message.txt' file, and save your Text Message.\n"
                       "Your Message can be consisted by any kind of characters and numbers.\n"
                       "Press 1 once you are done, or any key to exit this program\n");
        scanf("%s",answer);
        if (strcmp(answer,"1")!=0)
        {
            exit(0);
        }
    }
}

void read_ColourTableValues_File()
{
    FILE *file;
    char * word;
    char str[N]="";
    int i = 0,k=0;
    bool createHead = true;
    file = fopen("Colour_Table_Values.txt","r");

    if (file==NULL)
    {
        printf("\nCouldn't find the Text File\n");
        getch();
        exit(0);
    }

    while (fgets(str,N,file)!=NULL) //Read each line
    {
        word = strtok(str," "); //Split the line into words, based on the space character.
        while(word!=NULL)
        {
            for (i=0;i<strlen(word);i++) //Read each word's character
            {
                if (isdigit(word[i])==0) //Check if it is a number representation
                {
                    if (word[i]!='\n') // If we detect a number character and it's the line breaker print:
                    {
                        printf("\nInvalid Text File Input!\nPlease check that your values are only integers.");
                        getch();
                        exit(0);
                    }
                }
            }
            if (atoi(word)<0 || atoi(word)>255) //These are the accepted values for our Colour Table Values
            {
                printf("\nYour values are out of bounts!\nPlease make sure that you Colour Values are between 0-255.");
                getch();
                exit(0);
            }
            else
            {
                insert_to_File_List(atoi(word),createHead); //Store this word to a list
                createHead=false; //Do not create a new list head next time
                k++;
            }
            word = strtok(NULL," "); //Generate the next space-seperated word
        }
    }
    if (k==0)
    {
        printf("\nThe 'Colour_Table_Values.txt' file is empty.\n");
        getch();
        exit(0);
    }
    fclose(file);
    numberOfValues = k; //The amount of values that the user entered
}


void read_TextMessage_File()
{
    FILE *file = fopen("Text_Message.txt","r");
    int c,counter = 0;
    bool createHead = true;
    if (file == NULL)
    {
        printf("\nCouldn't find the Text File\n");
        getch();
        exit(0);
    }
    while ((c = fgetc(file)) != EOF)
    {
        counter++;
        insert_to_File_List(c,createHead); //Store this character to a list
        createHead = false;
    }
    if (counter==0)
    {
        printf("\nThe 'Text_Message.txt' file is empty.\n");
        getch();
        exit(0);
    }
    fclose(file);
    numberOfValues = counter;
}

// Implementing creation and insertion of the list that hosts the words/values from the txt file above
void insert_to_File_List(int word, bool createHead)
{
    if (createHead==true)
    {
        FHead = malloc(sizeof(struct ColourValues));
        FCurrent = FHead;
        FCurrent->value = word;
        FCurrent->next = NULL;
    }
    else
    {
        FNewNode = malloc(sizeof(struct ColourValues));
        FNewNode->value = word;
        FCurrent->next = FNewNode;
        FCurrent = FNewNode;
        FCurrent->next = NULL;
    }
}


// Starting the algorithm that finds the Frequency values of the Colour Table
void findAllValuesFrequency(CV FHead)
{
    static bool createHead;
    FCurrent = FHead; // Refers to the File List that hosts all the words/values from the txt file
    CHead = NULL;
    do
    {
        if (FCurrent==FHead) createHead = true;
        else createHead = false;
        findColourFrequency(FCurrent->value,createHead);
        FCurrent = FCurrent->next;
    }while(FCurrent!=NULL);
}


// For each colourValue that we give, this function finds it's frequency among all the values
void findColourFrequency(int colourValue, bool createHead)
{
    int CVFrequencyCounter = 0; //The frequency counter for each colourValue
    bool exists = false;
    FCurrent2 = FHead;
    if (CHead!=NULL)
    {
        CCurrent2 = CHead; //See UniqueList below

        /* In this part we check if the current colourValue's frequency has already been calculated.
         ie. the colour table may have multiple times the same value */
        do
        {
            if (CCurrent2->value==colourValue)
            {
                exists = true;
                break;
            }
            CCurrent2 = CCurrent2->next;
        }while(CCurrent2!=NULL);
    }

    if (exists==false) //If the current colourValue is new and hasn't been calculated before
    {
        do
        {
            if (FCurrent2->value==colourValue)
            {
                CVFrequencyCounter++; //Calculating the frequency
            }
            FCurrent2 = FCurrent2->next;
        }while(FCurrent2!=NULL);
        insert_to_VFList(colourValue,CVFrequencyCounter,createHead); //Storing it in our suitable List
        insert_to_Unique_List(colourValue,createHead); //Storing the 'already calculated values' here, in another list
    }
}

// Implementation of the final List that hold the unique colour Values with each one's frequency
void insert_to_VFList(int colourValue, int colourFrequency, bool createHead)
{
    if (createHead == true)
    {
        VFHead = malloc(sizeof(struct ValuesAndFrequencies));
        VFHead->value = colourValue;
        VFHead->frequency = colourFrequency;
        VFCurrent = VFHead;
        VFCurrent->next = NULL;
    }
    else
    {
        VFNewNode = malloc(sizeof(struct ValuesAndFrequencies));
        VFNewNode->value = colourValue;
        VFNewNode->frequency = colourFrequency;
        VFCurrent->next = VFNewNode;
        VFCurrent = VFNewNode;
        VFCurrent->next = NULL;
    }
}

// Implementation of the list that stores the already calculated values
void insert_to_Unique_List(int value, bool createHead)
{
    if (createHead==true)
    {
        CHead = malloc(sizeof(struct ColourValues));
        CCurrent = CHead;
        CCurrent->value = value;
        CCurrent->next = NULL;
    }
    else
    {
        CNewNode = malloc(sizeof(struct ColourValues));
        CNewNode->value = value;
        CCurrent->next = CNewNode;
        CCurrent = CNewNode;
        CCurrent->next = NULL;
    }
}

TRE create_New_TreeNode()
{
    TreeNodeNew = malloc(sizeof(struct nodeTree));
    TreeNodeNew->left = NULL;
    TreeNodeNew->right = NULL;
    return TreeNodeNew;
}

void add_To_LiveNodes_List(TRE newLiveNode)
{
    liveNodeListLength++;
    LiveNodeNew = malloc(sizeof(struct liveNodeList));
    LiveNodeNew->value = malloc(sizeof(struct nodeTree));
    LiveNodeNew->value = newLiveNode;
    LiveNodeNew->next = NULL;

    LiveNodeCurrent2 = LiveNodeHead;
    //If there is no live node left in the list, create new head
    if (LiveNodeCurrent2==NULL)
    {
        LiveNodeHead = malloc(sizeof(struct liveNodeList));
        LiveNodeHead->value = malloc(sizeof(struct nodeTree));
        LiveNodeHead->next = NULL;
        LiveNodeHead = LiveNodeNew;
    }
    else if (LiveNodeNew->value->frequency < LiveNodeCurrent2->value->frequency)
    {
        LiveNodeHead = LiveNodeNew;
        LiveNodeHead->next = LiveNodeCurrent2;
    }
    else if (LiveNodeNew->value->frequency < LiveNodeCurrent->value->frequency)
    {
        // Sorting the new live node
        while (LiveNodeCurrent2 != NULL && LiveNodeCurrent2->next != NULL)
        {
            if (LiveNodeNew->value->frequency >= LiveNodeCurrent2->value->frequency &&
                LiveNodeNew->value->frequency < LiveNodeCurrent2->next->value->frequency)
            {
                LiveNodeNew->next = LiveNodeCurrent2->next;
                LiveNodeCurrent2->next = LiveNodeNew;
            }
            LiveNodeCurrent2 = LiveNodeCurrent2->next;
        }
    }
    else if (LiveNodeNew->value->frequency >= LiveNodeCurrent->value->frequency)
    {
        LiveNodeCurrent->next = LiveNodeNew;
        LiveNodeCurrent = LiveNodeNew;
        LiveNodeCurrent->next = NULL;
    }
}


void remove_From_LiveNodes_List(TRE oldLiveNode)
{
    LiveNodeCurrent2 = LiveNodeHead;
    if (LiveNodeHead->value==oldLiveNode)
    {
        LiveNodeHead = LiveNodeHead->next;
        free(LiveNodeCurrent2);

    }
    else
    {
        LiveNodePrevious = LiveNodeCurrent2;
        LiveNodeCurrent2 = LiveNodeHead->next;
        while(LiveNodeCurrent!=NULL)
        {
            if (oldLiveNode==LiveNodeCurrent->value)
            {
                LiveNodePrevious->next = LiveNodeCurrent->next;
                break;
            }
            LiveNodePrevious = LiveNodeCurrent;
            LiveNodeCurrent = LiveNodeCurrent->next;
        }
    }
}


void initialize(TRE firstnode, TRE freqArray[], int loopIndex)
{
    /* We create our very first tree huffman node using the first two frequencies of our array
        We choose the two first array values, because at the beginning of the huffman coding
        it is certain that a new node from 2 freqs in gonna be created */
    firstnode->frequency = freqArray[loopIndex]->frequency + freqArray[loopIndex+1]->frequency;
    freqArray[loopIndex]->right = firstnode;
    freqArray[loopIndex+1]->left = firstnode;
    /* Index is a variable that helps us decide which way(left or right) to connect our nodes, in the case that
        we have two of them to connect */
    firstnode->index = loopIndex;
    /*Storing our node to the List of Live Nodes
        LivesNodesList is a list in which we dynamically keep all the nodes that are active in every every moment
        i.e. we do not want to compare and check nodes that are fully connected to another for example.
        We only care about the Up-Front ones - See theory anti-tree graph */
    LiveNodeHead = malloc(sizeof(struct liveNodeList));
    LiveNodeHead->value = malloc(sizeof(struct nodeTree));
    LiveNodeHead->value = firstnode;
    LiveNodeHead->next = NULL;
    LiveNodeCurrent = LiveNodeHead;
    liveNodeListLength++;
}


// This function calculated the two most minimum values from a bunch of values that are inserted
int minValuesGenerator(TRE array[], int loopIndex, int listlength)
{
    int result1,result2;
    int a1,a2,a3,a4; //The 4 most minimum values for potential treeNode making.
    int exitCode; /*Using custom coding system in order to build the nodeTree:
      node+node->22 | node+freq->21 | freq+node->12 | freq+freq->11 */

    /* We are making here, some caution checks to prevent null exceptions ect.
        Also, we are initializing our values */
    if (loopIndex<listlength)
    {
        a1 = array[loopIndex]->frequency;
    }
    else
    {
        a1 = INT_MAX;
    }

    if (loopIndex+1<=listlength-1)
    {
        a2 = array[loopIndex+1]->frequency;
    }
    else
    {
        a2 = INT_MAX;
    }
    a3 = LiveNodeHead->value->frequency;

    if (LiveNodeHead->next!=NULL)
    {
        a4 = LiveNodeHead->next->value->frequency;

    }
    else
    {
        a4 = INT_MAX;
    }

    //The 'two most minimum values' algorithm begins
    if (a1>=a4)
    {
        result1 = a3;
        result2 = a4;
    }
    else
    {
        if (a3>=a2)
        {
            result1 = a1;
            result2 = a2;
        }
        else
        {
            result1 = a1;
            result2 = a3;
        }
    }

    /* After we find out mins, we declare them as the chosen ones
        (either it is a node or an array freq value) to be connected to the new tree node that will be created later
        Also, we declare out exit-code here, in order to know what we are about to connect */
    if (result1==a3)
    {
        selectedNode1 = LiveNodeHead->value;
        if (result2==a4)
        {
            selectedNode2 = LiveNodeHead->next->value;
            exitCode = 22;
        }
        else
        {
            selectedFreq1 = array[loopIndex];
            exitCode = 21;
        }
    }
    else
    {
        selectedFreq1 = array[loopIndex];
        selectedFreq1->index = loopIndex;
        if (result2==a3)
        {
            selectedNode1 = LiveNodeHead->value;
            exitCode = 12;
        }
        else
        {
            selectedFreq2 = array[loopIndex+1];
            selectedFreq2->index = loopIndex+1;
            exitCode = 11;
        }
    }
    return exitCode;
}


void createTree()
{
    int I = 0; //Iteration counter. Indicates how many freq have been used so far.
    int listLength = findListLength(VFHead); //Find length of 'values and frequency' list
    int status; //Stores the exit status of minValuesGenerator
    TRE array[listLength]; //Creating an array of Tree nodes at the size of our VF list, aka number of our values
    TRE newnode;
    bool endLoop = false;
    insert_List_to_Array(array, VFHead); //See function
    newnode = create_New_TreeNode();
    initialize(newnode, array, I); //See function
    I+=2;

    while(!endLoop)
    {
        status = minValuesGenerator(array,I,listLength); //Calculating status

        // node+node->22 | node+freq->21 | freq+node->12 | freq+freq->11
        if (status==11)
        {
            I+=2; // We are using two elements of the array, so next time check further of them
            newnode = create_New_TreeNode();
            newnode->frequency = selectedFreq1->frequency + selectedFreq2->frequency;
            newnode->index = selectedFreq1->index;
            selectedFreq1->right = newnode;
            selectedFreq2->left = newnode;
        }
        else if (status==21 || status==12)
        {
            I++;
            newnode = create_New_TreeNode();
            newnode->frequency = selectedNode1->frequency + selectedFreq1->frequency;
            newnode->index = selectedNode1->index;
            selectedNode1->right = newnode;
            selectedFreq1->left = newnode;
            remove_From_LiveNodes_List(selectedNode1);
            /* We remove the current node because, since we created a
                new node from a previous one, we have to delete him for the Live Nodes List, since it's no longer
                Up-Front Active to be candidate again */
        }
        else if (status==22)
        {
            newnode = create_New_TreeNode();
            newnode->frequency = selectedNode1->frequency + selectedNode2->frequency;
            // Here we are using our 'index variable' methodology to decide in which we should connect our two nodes.
            if (selectedNode1->index < selectedNode2->index)
            {
                selectedNode1->right = newnode;
                selectedNode2->left = newnode;
                newnode->index = selectedNode2->index;
            }
            else
            {
                selectedNode2->right = newnode;
                selectedNode1->left = newnode;
                newnode->index = selectedNode1->index;
            }
            remove_From_LiveNodes_List(selectedNode1);
            remove_From_LiveNodes_List(selectedNode2);
        }

        /* The new tree node that will come up from one of the above calculations
            will be added to the live node list */
        add_To_LiveNodes_List(newnode);

        // When a node this frequency that is equal to the amount of values stop. Possibility = 1
        if (newnode->frequency==numberOfValues)
        {
            endLoop = true;
        }
    }
    printCodes(array);
}

// Printing the Huffman Outputs for each value
void printCodes(TRE array[])
{
    int k=0,l,i,len;
    l = findListLength(VFHead);
    for (k=0;k<l;k++)
    {
        if (textMode)
        {
            printf("\n\t\tORIGINAL VALUE:-> %c\tENCODED VALUE:->",(char) array[k]->value);
        }
        else
        {
            printf("\n\t\tORIGINAL VALUE:-> %d\tENCODED VALUE:->",array[k]->value);
        }
        tree_Traversal(array[k]);
        for (i=strlen(codeString);i>=0;i--) //Using -- cause we scanned the tree from bottom to top!
        {
            printf("%c", codeString[i]);
            // Saving this code to a string
            len = strlen(completeCode);
            completeCode[len] = codeString[i];
            completeCode[len+1] = '\0';
        }
        insertCodeToList(array[k]->value,completeCode);
        strcpy(completeCode,""); // Making this temporary string empty, in order to save the next here
        strcpy(codeString,""); //Making the string empty in order to calculate the new code
    }
    printf("\n\n");
    printf("Original Message: ");
    printOriginalMessage(textMode);
    printf("\n\nEncoded Message: ");
    printEncodedMessage();
    printf("\n\nSuccess!\n");
}


// Matching the codes to the File list
void insertCodeToList(int value,char* code)
{
    FCurrent2 = FHead;
    do
    {
        if (FCurrent2->value==value)
        {
            FCurrent2->code = (char*) malloc(12*sizeof(char));
            strcpy(FCurrent2->code,code);
        }
        FCurrent2 = FCurrent2->next;
    }while(FCurrent2!=NULL);
}


void printEncodedMessage()
{
    FCurrent2 = FHead;
    do
    {
        printf("%s",FCurrent2->code);
        FCurrent2 = FCurrent2->next;
    }while(FCurrent2!=NULL);
}

/* Problem with 'pow' function to be solved
void printCompressionPercentage(double length)
{
    double base=2.0,i=0.0;
    int bits = 1;
    bool stop = false;
    do
    {
        if (length==pow(base,i))
        {
            bits = (int) i;
            stop = true;
        }
        else if (length > pow(base,i) && length < pow(base,i+1.0))
        {
            bits = (int) i+1;
            stop = true;
        }
        i+=2.0;
    }while(!stop);
    printf("%d",bits);
}
*/

void printOriginalMessage(bool textMode)
{
    FCurrent2 = FHead;
    if (textMode)
    {
        do
        {
            printf("%c",(char) FCurrent2->value);
            FCurrent2 = FCurrent2->next;
        }while(FCurrent2!=NULL);
    }
    else
    {
        do
        {
            printf("%d ",FCurrent2->value);
            FCurrent2 = FCurrent2->next;
        }while(FCurrent2!=NULL);
    }
}


// Finding the huffman code of each value
void tree_Traversal(TRE t)
{
    if (t != NULL)
    {
        if (t->left != NULL)
        {
            strcat(codeString,"1");
            tree_Traversal(t->left);
        }
        else if (t->right != NULL)
        {
            strcat(codeString,"0");
            tree_Traversal(t->right);
        }
        else
        {
            if (t->frequency==numberOfValues)
            {
                tree_Traversal(NULL);
            }
            else
            {
                printf("something went wrong");
                tree_Traversal(NULL);
            }
        }
    }
}

int findListLength(VF head)
{
    int listLength=0;
    VFCurrent2 = head;
    do
    {
        if (VFCurrent2!=NULL)
        {
            listLength++;
        }
        VFCurrent2 = VFCurrent2->next;
    }while(VFCurrent2!=NULL);
    return listLength;
}

// Inserting the values of out Values&Frequencies list to an Array, for easier calculations
void insert_List_to_Array(TRE array[],VF head)
{
    int i = 0;
    VFCurrent2 = head;
    do
    {
        array[i] = malloc(sizeof(struct nodeTree));
        array[i]->left = NULL;
        array[i]->right = NULL;
        array[i]->frequency = VFCurrent2->frequency;
        array[i]->value = VFCurrent2->value;
        VFCurrent2 = VFCurrent2->next;
        i++;
    }while(VFCurrent2 != NULL);
    selectionSortArray(array,i);
}

// Sorting the array using the Selection Sort Algorithm
void selectionSortArray(TRE array[],int length)
{
    int i,d,position;
    TRE swap;
    for (i=0;i<length-1;i++)
    {
        position = i;
        for (d =i+1;d<length;d++)
        {
            if (array[position]->frequency > array[d]->frequency)
                position = d;
        }
        if (position!=i)
        {
            swap = array[i];
            array[i] = array[position];
            array[position] = swap;
        }
    }
}