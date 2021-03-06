#include <iostream>
#include <string>
#include <list>
using namespace std;

bool IS_PRINT = false;
void DEV_PRINT(string msg)
{
    if (IS_PRINT)
    {
        cout << msg << "\n";
    }
}

typedef int StructureItem;

class OperationResult
{
private:
    StructureItem item;
    bool isSuccesful;
    string errorInfo;
public:
    OperationResult()
    {}

    OperationResult(StructureItem item, bool isSuccesful)
        :item(item), isSuccesful(isSuccesful),errorInfo("ok")
    {}

    OperationResult(StructureItem item, bool isSuccesful, string errorInfo)
        :item(item), isSuccesful(isSuccesful), errorInfo(errorInfo)
    {}

    StructureItem getItem()
    {
        return this->item;
    }

    bool getResult()
    {
        return this->isSuccesful;
    }

    string getErrorInfo()
    {
        return this->errorInfo;
    }
};

class SimpleStructure 
{
protected:
    list<StructureItem>** structureList;
    bool* isStructureExist;
    const int maxAmountItemsPerStructure;
    const int structureAmount;
    const string structureName;
public:
    SimpleStructure(const int structureAmount, const int maxAmountItemsPerStructure, const string structureName)
        :structureAmount(structureAmount), maxAmountItemsPerStructure(maxAmountItemsPerStructure), structureName(structureName)
    {
        this->structureList = new list <StructureItem>*[structureAmount];
        this->isStructureExist = new bool[structureAmount]();
    }

    OperationResult createStructure(int structureIndex)
    {
        if (structureIndex >= 0 && structureIndex <= this->maxAmountItemsPerStructure)
        {
            this->isStructureExist[structureIndex] = true;
            this->structureList[structureIndex] = new list <StructureItem>;
            return OperationResult(structureIndex, true);
        }
        else
        {
            DEV_PRINT("createStructure bad index");
            return OperationResult(structureIndex, false, "createStructure bad index");
        }
    }

    OperationResult deleteStructure(int structureIndex)
    {
        if (this->CheckIsStructureIndexOK(structureIndex))
        {
            delete this->structureList[structureIndex];
            this->isStructureExist[structureIndex] = false;
            return OperationResult(structureIndex, true);
        }
        else
        {
            DEV_PRINT("deleteStructure bad index");
            return OperationResult(structureIndex, false, "deleteStructure bad index");
        }
    }

    bool CheckIsStructureIndexOK(int structureIndex)
    {
        // check is stack index correct
        //a) check is index beetween 0 and stackAmount
        //b) check is stack exist 
        if (structureIndex < this->structureAmount && structureIndex >= 0 && this->isStructureExist[structureIndex] == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    OperationResult checkCanAddElementToStructure(int structureIndex)
    {
        string errorInfo = "error: " + this->structureName + " is full\n";
        if (this->CheckIsStructureIndexOK(structureIndex))
        {

            if (this->structureList[structureIndex]->size() < this->maxAmountItemsPerStructure)
            {
                return OperationResult(structureIndex,true);
            }
            else
            {
                return OperationResult(structureIndex, false, errorInfo);
            }
        }
        else
        {
            DEV_PRINT("add element -> bad structure index");
            return OperationResult(structureIndex, false, errorInfo);
        }
    }

    OperationResult checkCanDeleteElementFromStructure(int structureIndex)
    {
        string errorInfo = "error: " + this->structureName + " is empty\n";
        if (this->CheckIsStructureIndexOK(structureIndex))
        {
            if (this->structureList[structureIndex]->size() > 0)
            {
                return OperationResult(structureIndex, true);
            }
            else
            {
                return OperationResult(structureIndex, false, errorInfo);
            }
        }
        else
        {
            DEV_PRINT("delete element -> bad structure index");
            return OperationResult(structureIndex, false, errorInfo);
        }
    }
   
    OperationResult checkCanPrintStructure(int structureIndex)
    {
        string errorInfo = "empty\n";
        if (this->CheckIsStructureIndexOK(structureIndex))
        {
            if (this->structureList[structureIndex]->size() == 0)
            {
                return OperationResult(structureIndex, false, errorInfo);
            }
            else
            {
                return OperationResult(structureIndex, true);
            }
        }
        else
        {
            DEV_PRINT("bad index -> structure print");
            return OperationResult(structureIndex, false, errorInfo);
        }
    }

    virtual OperationResult addElementToStructure(int structureIndex, StructureItem elementToInsert) = 0;
    virtual OperationResult deleteElementFromStructure(int structureIndex) = 0;
    virtual OperationResult printStructure(int structureIndex) = 0;
};

class StackStructure: public SimpleStructure
{
public:
    StackStructure(const int structureAmount, const int maxAmountItemsPerStructure, const string structureName)
        :SimpleStructure(structureAmount, maxAmountItemsPerStructure, structureName)
    {}

    OperationResult addElementToStructure(int structureIndex, StructureItem elementToInsert)
    {
        OperationResult check1 = this->checkCanAddElementToStructure(structureIndex);
        if (check1.getResult())
        {
            this->structureList[structureIndex]->push_front(elementToInsert);
            return OperationResult(elementToInsert, true);
        }
        else {
            return OperationResult(elementToInsert, false, check1.getErrorInfo());
        }
    }

    OperationResult deleteElementFromStructure(int structureIndex)
    {
        OperationResult check1 = this->checkCanDeleteElementFromStructure(structureIndex);
        if (check1.getResult())
        {
            StructureItem deletedElement = this->structureList[structureIndex]->front();
            this->structureList[structureIndex]->pop_front();
            return OperationResult(deletedElement, true);
        }
        else
        {
            return OperationResult(structureIndex, false, check1.getErrorInfo());
        }
    }

    OperationResult printStructure(int structureIndex)
    {
        OperationResult check1 = this->checkCanPrintStructure(structureIndex);
        if (check1.getResult())
        {
            int size = this->structureList[structureIndex]->size();
            for (int i = 0; i < size; i++)
            {
                StructureItem tmp = this->structureList[structureIndex]->back();
                cout << tmp << " ";
                this->structureList[structureIndex]->pop_back();
                this->structureList[structureIndex]->push_front(tmp);
            }
            cout << "\n";
            return OperationResult(structureIndex, true);
        }
        else
        {
            return OperationResult(structureIndex, false, check1.getErrorInfo());
        }
    }
};

class QueueStructure : public SimpleStructure
{
public:
    QueueStructure(const int structureAmount, const int maxAmountItemsPerStructure, const string structureName)
        :SimpleStructure(structureAmount, maxAmountItemsPerStructure, structureName)
    {}

    OperationResult addElementToStructure(int structureIndex, StructureItem elementToInsert)
    {
        OperationResult check1 = this->checkCanAddElementToStructure(structureIndex);
        if (check1.getResult())
        {
            this->structureList[structureIndex]->push_front(elementToInsert);
            return OperationResult(elementToInsert, true);
        }
        else {
            return OperationResult(elementToInsert, false, check1.getErrorInfo());
        }
    }

    OperationResult deleteElementFromStructure(int structureIndex)
    {
        OperationResult check1 = this->checkCanDeleteElementFromStructure(structureIndex);
        if (check1.getResult())
        {
            StructureItem deletedElement = this->structureList[structureIndex]->back();
            this->structureList[structureIndex]->pop_back();
            return OperationResult(deletedElement, true);
        }
        else
        {
            return OperationResult(structureIndex, false, check1.getErrorInfo());
        }
    }

    OperationResult printStructure(int structureIndex)
    {
        OperationResult check1 = this->checkCanPrintStructure(structureIndex);
        if (check1.getResult())
        {
            int size = this->structureList[structureIndex]->size();
            for (int i = 0; i < size; i++)
            {
                StructureItem tmp = this->structureList[structureIndex]->front();
                cout << tmp << " ";
                this->structureList[structureIndex]->pop_front();
                this->structureList[structureIndex]->push_back(tmp);
            }
            cout << "\n";
            return OperationResult(structureIndex, true);
        }
        else 
        {
            return OperationResult(structureIndex, false, check1.getErrorInfo());
        }
    }
};

class StructuresControler
{
private:
    QueueStructure queue;
    StackStructure stack;
public:
    StructuresControler(QueueStructure& q1, StackStructure& s1)
        :queue(q1), stack(s1)
    {}

    list<string> splitString(string text, string separator)
    {
        int  foundSeparator = text.find(separator);
        int indexStart = 0;
        list <string> arguments;
        if (foundSeparator != -1)
        {
            while (true)
            {
                string textToPush = text.substr(indexStart, foundSeparator - indexStart);
                arguments.push_back(textToPush);
                text = text.substr(foundSeparator + 1);
                foundSeparator = text.find(separator);
                if (foundSeparator == -1)
                {
                    arguments.push_back(text);
                    break;
                }
            }
        }
        return arguments;
    }

    int returnNextArgument(list<string>& command)
    {
        string argument1 = command.front();
        command.pop_front();
        int value1 = stoi(argument1);
        return value1;
    }

    void frontedCore()
    {
        list<string> commandListToExecute;

        while (true)
        {
            string commandToExecute;
            getline(cin, commandToExecute);
            if (commandToExecute == "")
            {
                break;
            }
            commandListToExecute.push_back(commandToExecute);
        }

        for (auto simpleCommand : commandListToExecute)
        {
            list<string> commandArguments = this->splitString(simpleCommand, " ");
            string operationType = commandArguments.front();
            commandArguments.pop_front();

            if (operationType == "new_s")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                this->stack.createStructure(arg1);
            }
            else if (operationType == "push")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                StructureItem arg2 = returnNextArgument(commandArguments);
                OperationResult result = this->stack.addElementToStructure(arg1, arg2);
                if (!result.getResult())
                {
                    cout << result.getErrorInfo();
                }
            }
            else if (operationType == "pop")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                OperationResult result = this->stack.deleteElementFromStructure(arg1);
                if (!result.getResult())
                {
                    cout << result.getErrorInfo();
                }
            }
            else if (operationType == "delete_s")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                this->stack.deleteStructure(arg1);
            }
            else if (operationType == "print_s")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                OperationResult result =  this->stack.printStructure(arg1);
                if (!result.getResult())
                {
                    cout << result.getErrorInfo();
                }
            }

            //queue if
            else if (operationType == "new_q")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                this->queue.createStructure(arg1);
            }
            else if (operationType == "enqueue")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                StructureItem arg2 = returnNextArgument(commandArguments);
                OperationResult result = this->queue.addElementToStructure(arg1, arg2);
                if (!result.getResult())
                {
                    cout << result.getErrorInfo();
                }
            }
            else if (operationType == "dequeue")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                OperationResult result = this->queue.deleteElementFromStructure(arg1);
                if (!result.getResult())
                {
                    cout << result.getErrorInfo();
                }
            }
            else if (operationType == "delete_q")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                this->queue.deleteStructure(arg1);
            }
            else if (operationType == "print_q")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                OperationResult result = this->queue.printStructure(arg1);
                if (!result.getResult())
                {
                    cout << result.getErrorInfo();
                }
            }

            //OPERATION MOVE
            else if (operationType == "queue->queue")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                StructureItem arg2 = returnNextArgument(commandArguments);
                
                if (this->queue.checkCanDeleteElementFromStructure(arg1).getResult() &&
                    this->queue.checkCanAddElementToStructure(arg2).getResult())
                {
                    OperationResult result = this->queue.deleteElementFromStructure(arg1);
                    this->queue.addElementToStructure(arg2, result.getItem());
                }
                else
                {
                    cout << "error: wrong command\n";
                }
            }
            else if (operationType == "stack->queue")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                StructureItem arg2 = returnNextArgument(commandArguments);
                
                if (this->stack.checkCanDeleteElementFromStructure(arg1).getResult() &&
                    this->queue.checkCanAddElementToStructure(arg2).getResult())
                {
                    OperationResult result = this->stack.deleteElementFromStructure(arg1);
                    this->queue.addElementToStructure(arg2, result.getItem());
                }
                else
                {
                    cout << "error: wrong command\n";
                }
            }
            else if (operationType == "queue->stack")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                StructureItem arg2 = returnNextArgument(commandArguments);
                
                if (this->queue.checkCanDeleteElementFromStructure(arg1).getResult() &&
                    this->stack.checkCanAddElementToStructure(arg2).getResult())
                {
                    OperationResult result = this->queue.deleteElementFromStructure(arg1);
                    this->stack.addElementToStructure(arg2, result.getItem());
                }
                else
                {
                    cout << "error: wrong command\n";
                }
            }
            else if (operationType == "stack->stack")
            {
                StructureItem arg1 = returnNextArgument(commandArguments);
                StructureItem arg2 = returnNextArgument(commandArguments);

                if (this->stack.checkCanDeleteElementFromStructure(arg1).getResult() &&
                    this->stack.checkCanAddElementToStructure(arg2).getResult())
                {
                    OperationResult result = this->stack.deleteElementFromStructure(arg1);
                    this->stack.addElementToStructure(arg2, result.getItem());
                }
                else
                {
                    cout << "error: wrong command\n";
                }
            }

        }

    }

};

        //--------------
        //test-1
        //--------------
        /*commandListToExecute.push_back("new_s 0"); 
        commandListToExecute.push_back("push 0 96");
        commandListToExecute.push_back("new_s 5");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("push 5 28");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("push 5 88");
        commandListToExecute.push_back("pop 0");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("push 0 65");
        commandListToExecute.push_back("stack->stack 5 0");
        commandListToExecute.push_back("print_s 0");*/


        //--------------
        //test-2
        //--------------
        //commandListToExecute.push_back("new_s 0");
        //commandListToExecute.push_back("push 0 96");
        //commandListToExecute.push_back("new_s 5");
        //commandListToExecute.push_back("print_s 5");
        //commandListToExecute.push_back("push 5 28");
        //commandListToExecute.push_back("push 5 99");
        //commandListToExecute.push_back("push 5 33");
        //commandListToExecute.push_back("push 5 88");
        //commandListToExecute.push_back("pop 0");
        //commandListToExecute.push_back("print_s 5");
        //commandListToExecute.push_back("pop 0");
        //commandListToExecute.push_back("push 0 65");
        //commandListToExecute.push_back("push 5 99");
        //commandListToExecute.push_back("push 5 13");
        //commandListToExecute.push_back("push 5 99");
        //commandListToExecute.push_back("push 5 1");
        //commandListToExecute.push_back("push 5 99");
        //commandListToExecute.push_back("push 5 0");
        //commandListToExecute.push_back("push 5 9");
        //commandListToExecute.push_back("push 5 87");
        //commandListToExecute.push_back("stack->stack 5 0");
        //commandListToExecute.push_back("print_s 0");


         //--------------
        //test-3
        //--------------
        /*commandListToExecute.push_back("new_s 0");
        commandListToExecute.push_back("push 0 96");
        commandListToExecute.push_back("new_s 5");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("push 5 28");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("new_q 0");
        commandListToExecute.push_back("push 5 33");
        commandListToExecute.push_back("push 5 88");
        commandListToExecute.push_back("pop 0");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("pop 0");
        commandListToExecute.push_back("push 0 65");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("dequeue 0");
        commandListToExecute.push_back("enqueue 0 4");
        commandListToExecute.push_back("new_q 9");
        commandListToExecute.push_back("push 5 13");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("enqueue 0 43");
        commandListToExecute.push_back("enqueue 0 21");
        commandListToExecute.push_back("enqueue 0 17");
        commandListToExecute.push_back("enqueue 0 4");
        commandListToExecute.push_back("enqueue 9 0");
        commandListToExecute.push_back("enqueue 0 4");
        commandListToExecute.push_back("enqueue 0 43");
        commandListToExecute.push_back("enqueue 0 40");
        commandListToExecute.push_back("push 5 1");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("enqueue 0 33");
        commandListToExecute.push_back("enqueue 0 99");
        commandListToExecute.push_back("enqueue 0 8");
        commandListToExecute.push_back("push 5 0");
        commandListToExecute.push_back("push 5 9");
        commandListToExecute.push_back("delete_q 0");
        commandListToExecute.push_back("print_q 9");
        commandListToExecute.push_back("push 5 87");
        commandListToExecute.push_back("new_q 0");
        commandListToExecute.push_back("enqueue 0 19");
        commandListToExecute.push_back("print_q 0");
        commandListToExecute.push_back("stack->stack 5 0");
        commandListToExecute.push_back("print_s 0");*/


        //--------------
       //test-3 only queue
       //--------------

        //commandListToExecute.push_back("new_q 0");
        //commandListToExecute.push_back("dequeue 0");
        //commandListToExecute.push_back("enqueue 0 4");
        //commandListToExecute.push_back("new_q 9");//
        //commandListToExecute.push_back("enqueue 0 43");
        //commandListToExecute.push_back("enqueue 0 21");
        //commandListToExecute.push_back("enqueue 0 17");
        //commandListToExecute.push_back("enqueue 0 4");
        //commandListToExecute.push_back("enqueue 9 0");
        //commandListToExecute.push_back("enqueue 0 4");
        //commandListToExecute.push_back("enqueue 0 43");
        //commandListToExecute.push_back("enqueue 0 40");
        //commandListToExecute.push_back("enqueue 0 33");
        //commandListToExecute.push_back("enqueue 0 99");
        //commandListToExecute.push_back("enqueue 0 8");
        //commandListToExecute.push_back("delete_q 0");
        //commandListToExecute.push_back("print_q 9");
        //commandListToExecute.push_back("new_q 0");
        //commandListToExecute.push_back("enqueue 0 19");
        //commandListToExecute.push_back("print_q 0");

        //--------------
       //test-4
       //--------------
        /*commandListToExecute.push_back("new_s 0");
        commandListToExecute.push_back("push 0 96");
        commandListToExecute.push_back("new_s 5");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("push 5 28");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("new_q 0");
        commandListToExecute.push_back("push 5 33");
        commandListToExecute.push_back("push 5 88");
        commandListToExecute.push_back("pop 0");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("pop 0");
        commandListToExecute.push_back("push 0 65");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("dequeue 0");
        commandListToExecute.push_back("enqueue 0 4");
        commandListToExecute.push_back("new_q 9");
        commandListToExecute.push_back("push 5 13");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("enqueue 0 43");
        commandListToExecute.push_back("enqueue 0 21");
        commandListToExecute.push_back("enqueue 0 17");
        commandListToExecute.push_back("stack->queue 0 0");
        commandListToExecute.push_back("enqueue 0 4");
        commandListToExecute.push_back("stack->queue 0 0");
        commandListToExecute.push_back("enqueue 9 0");
        commandListToExecute.push_back("enqueue 0 4");
        commandListToExecute.push_back("enqueue 0 43");
        commandListToExecute.push_back("queue->queue 0 0");
        commandListToExecute.push_back("stack->stack 5 5");
        commandListToExecute.push_back("enqueue 0 40");
        commandListToExecute.push_back("push 5 1");
        commandListToExecute.push_back("push 5 99");
        commandListToExecute.push_back("enqueue 0 33");
        commandListToExecute.push_back("enqueue 0 99");
        commandListToExecute.push_back("enqueue 0 8");
        commandListToExecute.push_back("push 5 0");
        commandListToExecute.push_back("push 5 9");
        commandListToExecute.push_back("delete_q 0");
        commandListToExecute.push_back("print_q 9");
        commandListToExecute.push_back("push 5 87");
        commandListToExecute.push_back("new_q 0");
        commandListToExecute.push_back("stack->queue 5 0");
        commandListToExecute.push_back("enqueue 0 3");
        commandListToExecute.push_back("queue->queue 0 0");
        commandListToExecute.push_back("enqueue 0 19");
        commandListToExecute.push_back("stack->stack 5 0");
        commandListToExecute.push_back("print_s 0");
        commandListToExecute.push_back("print_s 5");
        commandListToExecute.push_back("print_q 0");
        commandListToExecute.push_back("print_q 9");*/

int main()
{
    QueueStructure queue(10, 10, "queue");
    StackStructure stack(10, 10, "stack");
    StructuresControler controler(queue, stack);
    controler.frontedCore();
}
