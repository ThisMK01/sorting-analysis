#include <iostream>
#include <raylib.h>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

#define ScreenWidth 1000
#define ScreenHeight 700
#define minScreenWidth 500
#define minScreenHeight 600
#define fps 60
#define normal 0
#define sorted 1
#define selected 2


double Timetaken;
int id=0;
int numberofpillars = 0;
bool randommize = true;
bool shouldshowscreen = true;
bool bubblepress = false;
bool randomizepress = false;
bool quickpress = false;
bool selectionpress = false;
bool shouldstartsorting = true;
float speedFactor = 1.0; 




void quicksort(std::vector<std::pair<int, int>> &arr, int high ,int low);
int partiation(std::vector<std::pair<int, int>> &arr,int high , int low);
void showscreen();
void sortarr(int id);
void button(float x, float y, char *Text, Color color, bool &state);
void bubblebtn(float size, char bubble[]);
void quickbtn(float size, char quick[]);
void selectionbtn(float size, char selection[]);
void randomizebtn(float size, char randomize[]);
void bubbleSort(std::vector<std::pair<int, int>> &arr, int numberofpillars);

void draw(std::vector<std::pair<int, int>> &arr);
void random(std::vector<std::pair<int, int>> &arr);
Color FindColorForPILLer(int pillarstate);
void speedIncreaseBtn(float size, char increaseSpeed[]);
void speedDecreaseBtn(float size, char decreaseSpeed[]);
void WaitTime(float seconds);
std::vector<std::pair<int, int>> arr(numberofpillars);

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithm");
    SetWindowMinSize(minScreenWidth, minScreenHeight);
    SetTargetFPS(fps);

    std::string inputText = "";
    char textBuffer[256] = {0};
    int textSize = 0;
    bool inputActive = true;
    int number = 0;
    

    while (!WindowShouldClose()) {
        if (shouldshowscreen)
            showscreen();

        if (randommize)
            random(arr);
        randommize = false;

        if (shouldstartsorting)
            sortarr(id);
           shouldstartsorting=false;

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (textSize > 0) {
                textSize--;
                inputText.pop_back();
            }
        } else if (IsKeyPressed(KEY_ENTER)) {
            if (!inputText.empty()) {
                std::stringstream ss(inputText);
                ss >> number;
                numberofpillars = number;
                arr.resize(numberofpillars);
                inputText.clear();
                textSize = 0;
                randommize = true;
                std::cout << "Entered number: " << numberofpillars << std::endl;
            }
        } else {
            for (int key = KEY_ZERO; key <= KEY_NINE; key++) {
                if (IsKeyPressed(key)) {
                    char digit = '0' + (key - KEY_ZERO);
                    if (textSize < sizeof(textBuffer) - 1 && inputText.length() < 4) {
                        inputText += digit;
                        textBuffer[textSize] = digit;
                        textSize++;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Entered number:", GetScreenWidth() / 45, GetScreenHeight() / 30, (2 * GetScreenWidth()) / 100, RED);
        DrawRectangle(20, GetScreenHeight() / 6, (8 * GetScreenWidth()) / 100, (6 * GetScreenHeight()) / 100, LIGHTGRAY);
        DrawText(inputText.c_str(), (2.5 * GetScreenWidth()) / 100, (17 * GetScreenHeight()) / 100, (3 * GetScreenWidth()) / 100, RED);
        DrawText("only enter 4 digit \n\nand up to 8181", (2 * GetScreenWidth()) / 100, (8 * GetScreenHeight()) / 100, (2 * GetScreenWidth()) / 100, RED);
        DrawText(TextFormat("You entered: %d", numberofpillars), GetScreenWidth() / 45, 1.7 * GetScreenHeight() / 7, (2 * GetScreenWidth()) / 100, RED);
        
    
       
        speedIncreaseBtn(MeasureText("Increase Speed", (1.2 * GetScreenWidth()) / 100),"Increase Speed");
        speedDecreaseBtn(MeasureText("Decrease Speed", (1.2 * GetScreenWidth()) / 100),"Decrease Speed");
        std::stringstream sk;
        sk << std::fixed << std::setprecision(2) << Timetaken;
        std::string tracktext = "Time taken to sort: " + sk.str() + " seconds";
        DrawText(tracktext.c_str(), (2.7 * GetScreenWidth() / 4), (7.2 * GetScreenHeight() / 30), (2 * GetScreenWidth()) / 100, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void draw(std::vector<std::pair<int, int>> &arr) {
    float barwidth = (float)GetScreenWidth() / numberofpillars;
    for (int i = 0; i < numberofpillars; i++) {
        Color color = FindColorForPILLer(arr[i].second);
        DrawRectangleV(
            Vector2{(float)i * barwidth, (float)GetScreenHeight() - arr[i].first},
            Vector2{barwidth, (float)arr[i].first},
            color
        );
    }
}

void random(std::vector<std::pair<int, int>> &arr) {
    for (int i = 0; i < numberofpillars; i++) {
        arr[i] = { GetRandomValue(100, minScreenWidth - 100), normal };
    }
    return;
}

void showscreen() {
    float font = (1.5 * GetScreenWidth()) / 100;
    char randomize[] = "Randomize";
    char bubble[] = "BUBBLE SORT";
    char quick[] = "QUICK SORT";
    char selection[] = "SELECTION SORT";

    float tmpran = MeasureText(randomize, font);
    float tmpb = MeasureText(bubble, font);
    float tmpq = MeasureText(quick, font);
    float tmps = MeasureText(selection, font);

    randomizebtn(tmpran, randomize);
    bubblebtn(tmpb, bubble);
    quickbtn(tmpq, quick);
    selectionbtn(tmps, selection);
    draw(arr);
}

void randomizebtn(float size, char randomize[]) {
    Color color;
    if (randomizepress) {
        random(arr);
        randomizepress = false;
    } else
        color = RED;

    button(GetScreenWidth() / 3.3 - size, GetScreenHeight() / 12, randomize, color, randomizepress);
}

void bubblebtn(float size, char bubble[]) {
    Color color;
  
    if (bubblepress) {
        color = SKYBLUE;
        id=1;
        sortarr(id);
        bubblepress = false;
    } else
        color = RED;

    button(GetScreenWidth() / 3 - size, GetScreenHeight() / 30, bubble, color, bubblepress);
}

void quickbtn(float size, char quick[]) {
    Color color;
    
    if (quickpress) {
        color = SKYBLUE;
         id=2;
         sortarr(id);
         quickpress=false;
    } else
        color = RED;

    button(GetScreenWidth() / 2 - size, GetScreenHeight() / 30, quick, color, quickpress);
}

void selectionbtn(float size, char selection[]) {
    Color color;
    if (selectionpress) {
        color = SKYBLUE;
    } else
        color = RED;

    button(GetScreenWidth() / 1.4 - size, GetScreenHeight() / 30, selection, color, selectionpress);
}

void button(float x, float y, char *Text, Color color, bool &state) {
    float font = (2 * GetScreenWidth() / 100);
    Rectangle r1 = {
        .x = x,
        .y = y,
        .width = (float)MeasureText(Text, font),
        .height = (float)font,
    };
    if (CheckCollisionPointRec(GetMousePosition(), r1)) {
        DrawText(Text, x, y, font, GREEN);
        if (IsMouseButtonPressed(0)) {
            state = !state;
            return;
        }
    } else {
        DrawText(Text, x, y, font, color);
    }
    return;
}

Color FindColorForPILLer(int pillarstate) {
    switch (pillarstate) {
    case selected:
        return WHITE;
        break;
    case sorted:
        return GREEN;
        break;
    default:
        return GOLD;
        break;
    }
}



void bubbleSort(std::vector<std::pair<int, int>> &arr, int numberofpillars) {
    int n = numberofpillars;
    bool swapped;
     double stime=GetTime();
    do {
        swapped = false;
        for (int j = 0; j < n - 1; j++) {
            arr[j].second = selected;
            arr[j + 1].second = selected;
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
            
            BeginDrawing();
            ClearBackground(BLACK);

            for (int i = numberofpillars - 1; i >= n; i--) {
                arr[i].second = sorted;
            }

            draw(arr);
            
            EndDrawing();

            WaitTime(0.05f);

            arr[j].second = normal;
            arr[j + 1].second = normal;
        }
        n--;
    } while (swapped);

    for (int i = numberofpillars - 1; i >= 0; i--) {
        arr[i].second = sorted;
    }
    double etime=GetTime();
    double track = etime-stime;
    Timetaken = track;
}


    int partiation(std::vector<std::pair<int, int>> &arr,int high , int low)
    {
        int pivot=arr[high].first;
        int i=low-1;
        for(int j = low; j<high ; j++)
        {
            if(arr[j].first < pivot)
            {
                i++;
                std::swap(arr[i],arr[j]);
            }
            arr[j].second = selected; 
            arr[i].second=selected;
            BeginDrawing();
            ClearBackground(BLACK);
           
            draw(arr);
            EndDrawing();


            arr[j].second = normal; 
        }
        std::swap(arr[i + 1], arr[high]);
          arr[i + 1].second = sorted;
        
        return i + 1;
    }

   void quicksort(std::vector<std::pair<int, int>> &arr, int high ,int low)
   {
      double stime=GetTime();
      if(low<high)
      {
       int p = partiation(arr,high,low);
       quicksort(arr,p-1,low);
       quicksort(arr,high,p+1);
        for (int i = low; i <= high; i++) {
            arr[i].second = sorted;
        }
      }
       double etime=GetTime();
    double track = etime-stime;
    Timetaken = track;
     
   }






void speedIncreaseBtn(float size, char increaseSpeed[]) {
    Color color = RED;
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){GetScreenWidth() / 3.3f - size, GetScreenHeight() / 9, size, (float)(2 * GetScreenWidth()) / 100})) {
        color = GREEN;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (speedFactor > 0.1) {
                speedFactor -= 0.1f; // Increase speed by decreasing the delay
            }
        }
    }
    DrawText(increaseSpeed, GetScreenWidth() / 3.3f - size, GetScreenHeight() / 9, (2 * GetScreenWidth()) / 100, color);
}

void speedDecreaseBtn(float size, char decreaseSpeed[]) {
    Color color = RED;
     
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){GetScreenWidth() / 3.3f - size, GetScreenHeight() / 7.5, size, (float)(2 * GetScreenWidth()) / 100})) {
        color = GREEN;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            speedFactor += 0.1f; // Decrease speed by increasing the delay
        }
    }
    DrawText(decreaseSpeed, GetScreenWidth() / 3.3f - size, GetScreenHeight() / 7.5, (2 * GetScreenWidth()) / 100, color);
}

void WaitTime(float seconds) {
    double startTime = GetTime();
    while ((GetTime() - startTime) < seconds * speedFactor) {
        // Do nothing, just wait
    }
}
void sortarr(int id) {
    if(id==1){
    bubbleSort(arr, numberofpillars);
    }
    else if(id==2){
    
    quicksort(arr,numberofpillars-1,0);
    }
    shouldstartsorting = false;
    draw(arr);


    bubblepress = false;
    quickpress = false;

}