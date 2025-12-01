#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
void printMenu();
void printBook();
void printBorrow();

struct Date { 
	int day ; 
	int month ; 
	int year ; 
};
typedef struct Date DE ; 

struct Book{ 
	int bookId ; 
	char title[50] ; 
	char author[50] ; 
	int publishYear ; 
	int quantity ; 
};
typedef struct Book BK ; 
int autoId = 1 ; 
int length = 0 ;
BK menu[1000];
BK newBook[1000]; 


struct Borrow{ 
	int borrowId ; 
	int bookId ; 
	DE borrowDate ; 
	DE borrowReturn ; 
	char borrowerName[50] ; 
	int status  ; 
};
typedef struct Borrow BR ; 
int currentId = 1 ;
BR borrowBook[1000]; 
int lengthBR = 0 ; 

void deleteCharactor(char *string ,int index){ 
	int length = strlen(string); 
	for(int i = index ; i<length - 1 ; i++ ){ 
		string[i]=string[i+1]; 
	}
	string[length -1]='\0'; 
}
void removeSpaces(char *str){ 
	while(str[0]==' '&& strlen(str)>0){ 
		deleteCharactor(str,0); 
	}
}
void trim(char *str){
    int start = 0;
    while(str[start] == ' ') start++;

    int end = strlen(str) - 1;
    while(end >= start && str[end] == ' ') end--;

    int j = 0;
    for(int i = start; i <= end; i++){
        str[j++] = str[i];
    }
    str[j] = '\0';
}
void getString(char *string ,char suggest[] , int size){ 
	do{ 
		printf("%s : ",suggest); 
		fgets(string,size,stdin); 
		string[strcspn(string,"\n")]='\0'; 
		
		if(strlen(string)==0){ 
			printf("Khong de trong !!!Vui long nhap!!!\n"); 
		}else if(string[0]==' '){ 
			removeSpaces(string); 
			printf("Khong de trong !!!Vui long nhap!!!\n"); 
		}
	}while(strlen(string)==0); 
}
int getNumber(char suggest[], int min, int max){ 
    char input[100]; 
    int numberBook; 
    int valid; 
    
    do{ 
        printf("%s : ", suggest); 
        
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        
        trim(input);

        valid = 1;

         
        int onlySpace = 1;
        for(int i = 0; input[i] != '\0'; i++){
            if(input[i] != ' '){
                onlySpace = 0;
                break;
            }
        }

        if(strlen(input) == 0 || onlySpace){
            printf("Khong duoc de trong! Vui long nhap so.\n");
            valid = 0;
            continue;
        }

        
        int start = 0;
        if(input[0] == '-' || input[0] == '+'){
            start = 1;
        }

        
        for(int i = start; input[i] != '\0'; i++){
            if(!isdigit((unsigned char)input[i])){
                valid = 0;
                break;
            }
        }

        if(!valid){
            printf("Khong hop le!!! Chi duoc nhap so nguyen!\n");
            continue;
        }

        numberBook = atoi(input);

        if(numberBook < min || numberBook > max){
            printf("Vui long nhap nam trong khoang %d den %d !!!\n", min, max);
            valid = 0;
        }

    }while(!valid);

    return numberBook;
}


int isBookBorrow(int bookId){ 
	for(int i = 0 ;i<lengthBR ; i++){ 
		if(borrowBook[i].bookId == bookId && borrowBook[i].status == 1 ){ 
				return 1 ; 
		}
	}
		return 0 ; 
} 

int confirmYN(char *choiceYN){ 
    char choice;
    printf("%s (y/n): ", choiceYN);
    scanf(" %c", &choice);
    while(getchar()!='\n'); // clear buffer

    if(choice == 'y' || choice == 'Y') return 1;
    return 0;
}

void deleteListBook(BK menu[],int *length){ 
	if(*length == 0 ){ 
		printf("Mang rong !!!Vui long nhap 1 \n"); 
		return ; 
	}
	int id ; 
	id = getNumber("Nhap id sach can xoa:",1,10000); 
	
	for(int i = 0 ; i<*length ; i++){ 
		
		if(menu[i].bookId == id){ 
			// kiem tra 
			if(isBookBorrow(menu[i].bookId)){ 
				printf("Khong the xoa sach !! Dang co nguoi muon\n"); 
				return ; 
			}
			printf("Ban co chac chan muon xoa sach \"%s\" khong?\n", menu[i].title);
            if(!confirmYN("Nhap lua chon")){
                printf("Huy xoa sach!\n");
                return;
            }
			for(int j = i; j<*length - 1 ; j++){ 
				menu[j] = menu[j+1]; 
			}
			(*length)-- ; 
			printf("Xoa sach thanh cong !!!!\n"); 
			return ; 
		}
	}
	printf("Khong tim thay sach!!!!\n"); 
}

void toLowerCase(char *str) { 
    for(int i = 0; str[i]; i++){
        str[i] = tolower((unsigned char)str[i]);
    }
}

void findListBook(BK menu[],int length){ 
    if(length == 0){
        printf("Chua co sach trong thu vien !!!!Vui long chon 1\n");
        return;
    }
    char key[50];
    getString(key,"Nhap ten sach muon tim",sizeof(key));

    
    char keyLower[50];
    strcpy(keyLower, key);
    toLowerCase(keyLower);

    BK *result[length];
    int count = 0;

    for(int i = 0; i < length; i++){
        char titleLower[50];
        strcpy(titleLower, menu[i].title);
        toLowerCase(titleLower);

        if(strstr(titleLower, keyLower)){
            result[count] = &menu[i];
            count++;
        }
    }

    if(count == 0){
        printf("Khong tim thay sach !!!!\n");
        return;
    }

    printf("\n===============Danh sach ket qua =============\n");
    for(int i = 0 ; i<count ; i++){
        printf("|IdBook : %d |NameBook: %s |Author: %s |PublishYear: %d |Quantity: %d |\n",
            result[i]->bookId,
            result[i]->title,
            result[i]->author,
            result[i]->publishYear,
            result[i]->quantity);
    }
}

int isDuplicateTitle(BK menu[], int length, char *title) {
    for (int i = 0; i < length; i++) {
        if (strcmp(menu[i].title, title) == 0) {
            return 1;  
        }
    }
    return 0;
}

void inputBook(BK menu[],int *length ){  
	int add ; 
	
	add = getNumber("Nhap so luong sach muon them",1,1000);  
	
	if (*length + add > 1000) {
    	printf("Vuot gioi han mang!\n");
    	return;
	}		
	for(int i =*length; i<*length + add ; i++){ 
		menu[i].bookId = autoId ++ ; 
		printf("ID SACH:%d\n",autoId-1);
		
		do {
    		getString(menu[i].title, "Nhap tieu de sach", sizeof(menu[i].title));

    		if (isDuplicateTitle(menu, *length, menu[i].title)) {
        		printf("Ten sach da ton tai! Vui long nhap lai.\n");
    		}
		} while (isDuplicateTitle(menu, *length, menu[i].title));
 
        getString(menu[i].author, "Nhap ten tac gia", sizeof(menu[i].author));

		
		menu[i].publishYear = getNumber("Nhap nam xuat ban",1900,2025); 
		menu[i].quantity = getNumber("Nhap so luong sach",1,100000); 
		
	}
	(*length)+=add ; 
	printf("Them thanh cong !!!!\n"); 
	
}

void printBookList(BK menu[],int length){ 
	if(length==0){ 
		printf("Mang rong!!!Vui long nhap 1 truoc\n"); 
		return ; 
	}
	 
	int pageSize = 10 ; 
	int totalPage = (length + pageSize -1 )/pageSize ; 
	int currentPage = 1 ; 
	
	
	while(1){ 
		printf("\n=================Danh sach sach (trang %d/%d)==============\n",currentPage,totalPage); 
		
		int start = (currentPage -1)*pageSize ; 
		int end  = start + pageSize ; 
		
		if(end > length){ 
			end = length ; 
		}
		for(int i = start ; i<end ; i++){ 
		printf("|IdBook : %d |NameBook: %s |Author: %s |PublishYear: %d |Quantity: %d |\n"
		,menu[i].bookId
		,menu[i].title
		,menu[i].author
		,menu[i].publishYear
		,menu[i].quantity); 
	}
		int choicePage ; 
		printf("(1).Quay lai |  (2).Thoat  | (3).Trang tiep\n"); 
		do{ 
			choicePage = getNumber("Nhap lua chon",1,3); 
			if(choicePage == 1){ 	
				if(currentPage>1){ 
					currentPage --; 
				}
			}else if(choicePage == 2 ){ 
				return  ; 
			}else if(choicePage == 3){ 
				currentPage ++ ; 
				if(currentPage > totalPage){ 
				printf("Da den gioi han trang!!!!\n"); 
				currentPage--; 
			}
			
			}
		}while(currentPage > totalPage);
	}	
}

void updateBook(BK menu[],int length){ 
	int id ; 
	if(length == 0){ 
		printf("Mang rong !!!Vui long chon 1 truoc!!!\n"); 
		return ; 
	}
	id = getNumber("Nhap id can tim",1,10000); 
	for(int i = 0 ; i<length;i++){ 
		if(menu[i].bookId == id ){ 
			printf("Tim thay id vui long sua !!!\n"); 
			getString(menu[i].title,"Nhap ten sach moi",sizeof(menu[i].title));
			getString(menu[i].author,"Nhap ten tac gia moi",sizeof(menu[i].author)); 
			
			menu[i].publishYear = getNumber("Nhap nam xuat ban moi",1900,2025); 
			menu[i].quantity = getNumber("Nhap so luong sach moi",1,100000000); 
			
			printf("Cap nhat thanh cong !!!!\n"); 
			return ; 
		}
	}
	printf("Khong tim thay sach!!!"); 
}



int findBookId(int bookId){ 
	for(int i = 0 ; i<length ; i++){ 
		if(menu[i].bookId==bookId){ 
			return i ; 
		}
		
	}
	return -1 ; 
}

int isValidDate(DE d){ 
    int dayInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    if(d.month < 1 || d.month > 12) return 0;
    if(d.year < 1900 || d.year > 2100) return 0;

    
    int leap = (d.year % 400 == 0) || (d.year % 4 == 0 && d.year % 100 != 0);
    if(leap && d.month == 2) return d.day >= 1 && d.day <= 29;

    return d.day >= 1 && d.day <= dayInMonth[d.month];
}

void addBorrow(){  
	if(length==0){ 
		printf("Khong co sach trong thu vien!!!vui long nhap 1 de them sach"); 
		return ; 
	}
	BR temp ;  
	temp.borrowId = currentId++ ;
	printf("Ma phieu muon so:%d\n",temp.borrowId); 
	
	temp.bookId = getNumber("Nhap ma sach muon",1,100000);  
	
	int index = findBookId(temp.bookId); 
	
	if(index == -1){ 
		printf("Khong tim thay sach!!!Them phieu muon that bai!\n"); 
		currentId -- ; 
		return ; 
	}
	if(menu[index].quantity<=0){ 
		printf("Het sach !!!Them phieu muon that bai!\n"); 
		currentId -- ; 
		return ; 
	}
	
	// nhap ngay muon sach 
	do{
    temp.borrowDate.day = getNumber("Nhap ngay muon",1,31);
    temp.borrowDate.month = getNumber("Nhap thang muon",1,12);
    temp.borrowDate.year = getNumber("Nhap nam muon",2023,2025);

    if(!isValidDate(temp.borrowDate)){
        printf("Ngay thang nam KHONG HOP LE !!! Vui long nhap lai.\n");
    } else {
        break;
    }

}while(1);
	// ngay tra sach de trong 
	temp.borrowReturn.day = 0 ; 
	temp.borrowReturn.month = 0 ; 
	temp.borrowReturn.year = 0 ; 
	if(temp.borrowDate.year < menu[index].publishYear){
    printf("Sach chua xuat ban, khong the muon!\n");
    currentId--;
    return;
	}
	// nhap ten nguoi muon 
	getString(temp.borrowerName,"Nhap ten nguoi muon",sizeof(temp.borrowerName)); 	

	// trang thai sach 
	temp.status = 1 ; 
	// luu sach vao phieu muon 
	borrowBook[lengthBR]=temp ; 
	lengthBR ++ ; 
	// giam so luon sach 
	menu[index].quantity -- ; 
	printf("Them phieu muon thanh cong !!!!"); 
}

int compareDate(DE a, DE b){  
    if(a.year != b.year){
    	return a.year - b.year;
	}
    if(a.month != b.month){ 
    	return a.month - b.month;
	} 
    return a.day - b.day;
}

void returnBook(){ 
    if(length==0){ 
        printf("Khong co sach trong thu vien!!!vui long nhap 1 de them sach\n"); 
        return ; 
    }

    int id = getNumber("Nhap ma phieu muon",1,10000); 
    int indexBR = -1; 

    for(int i = 0 ; i < lengthBR ; i++){ 
        if(borrowBook[i].borrowId == id){ 
            indexBR = i; 
            break; 
        }
    }

    if(indexBR == -1){ 
        printf("Khong tim thay phieu muon !!!!\n"); 
        return ; 
    }
    if(borrowBook[indexBR].status == 0){ 
        printf("Phieu da duoc tra truoc do !!!\n"); 
        return ; 
    }

    int bookIndex = findBookId(borrowBook[indexBR].bookId); 
    if(bookIndex == -1){ 
        printf("Sach khong ton tai trong kho!!!\n"); 
        return ; 
    }

    
    DE returnBK;
    do{
    returnBK.day = getNumber("Nhap ngay tra sach",1,31); 
    returnBK.month = getNumber("Nhap thang tra sach",1,12);
    returnBK.year = getNumber("Nhap nam tra sach",2025,2030);

    if(!isValidDate(returnBK)){
        printf("Ngay thang nam KHONG HOP LE !!! Vui long nhap lai.\n");
        continue;
    }

    if(compareDate(returnBK, borrowBook[indexBR].borrowDate) < 0){
        printf("Ngay tra sach phai sau hoac bang ngay muon!!!\n");
    } else {
        break;
    }

} while(1);

    borrowBook[indexBR].borrowReturn = returnBK;
    borrowBook[indexBR].status = 0; 
    menu[bookIndex].quantity++; 

    printf("Tra sach thanh cong !!!!\n"); 
}

void printBorrowList(){ 

	if(lengthBR==0){ 
		printf("Danh sach phieu muon trong!!!!"); 
		return ; 
	}
	
	int pageSize  = 10 ; 
	int totalPage = (lengthBR + pageSize - 1)/pageSize ; 
	int currentPage = 1 ; 
	
	while(1){ 
		 
		int start = (currentPage -1)*pageSize ; 
		int end = start + pageSize; 
		
		
		printf("\n========= DANH SACH PHIEU MUON(Trang %d/%d) =========\n",currentPage, totalPage); 
		for(int i = start ; i<end && i < lengthBR ; i++){ 
			printf("ID phieu : %d | ID sach : %d | Ten nguoi muon : %s|\n ",
			borrowBook[i].borrowId,
			borrowBook[i].bookId,
			borrowBook[i].borrowerName); 
		
			printf("Ngay muon :%02d/%02d/%04d\n"
			,borrowBook[i].borrowDate.day
			,borrowBook[i].borrowDate.month
			,borrowBook[i].borrowDate.year); 
		
			if(borrowBook[i].status ==1){ 
				printf("Trang thai : Dang muon\n"); 
			}else{ 
				printf("Trang thai : Da tra | Ngay tra :%02d/%02d/%04d\n"
				,borrowBook[i].borrowReturn.day
				,borrowBook[i].borrowReturn.month
				,borrowBook[i].borrowReturn.year);
		}
		printf("------------------------------------------------------\n");
	}	
	int choicePage ; 
	printf("1.Trang truoc | 2.Thoat | 3.Trang tiep|\n");  
	do{ 
			choicePage = getNumber("Nhap lua chon",1,3); 
			if(choicePage == 1){ 	
				if(currentPage>1){ 
					currentPage --; 
				}
			}else if(choicePage == 2 ){ 
				return  ; 
			}else if(choicePage == 3){ 
				currentPage ++ ; 
				if(currentPage > totalPage){ 
				printf("Da den gioi han trang!!!!\n"); 
				currentPage--; 
			}
			}
			
		}while(currentPage > totalPage);
	}
	
	
}

void printBook(){ 
	int enter  ;
	do{ 
		printf("\n=========================THONG TIN SACH===================\n"); 
		printf("||                  1.Them moi sach                     ||\n"); 
		printf("||                  2.Cap nhat thong tin sach           ||\n"); 
		printf("||                  3.Hien thi danh sach sach           ||\n"); 
		printf("||                  4.Xoa thong tin sach                ||\n"); 
		printf("||                  5.Tim kiem sach                     ||\n"); 
		printf("||                  6.Quay tro lai                      ||\n"); 
		printf("==========================================================\n"); 
		enter = getNumber("Nhap lua chon",1,6); 
		
		switch(enter){ 
			case 1: { 
				inputBook(menu,&length); 
				break;
			}
			case 2 :{ 
				updateBook(menu,length); 
				break;
			}
			case 3 : { 
				printBookList(menu,length); 
				break;
			}
			case 4 : { 
				deleteListBook(menu,&length); 
				break;
			}
			case 5 : { 
				findListBook(menu,length); 
				break;
			}
			case 6 : { 
				return ; 
			}
			default : { 
				printf("Vui long lua chon lai!!!!"); 
				break;
			}
		}
	}while(enter != 6) ; 
}

void printBorrow(){ 
	int enter  ; 
	do{ 
		printf("\n========================PHIEU MUON=======================\n"); 
		printf("||                  1.Them moi phieu muon              ||\n"); 
		printf("||                  2.Tra sach                         ||\n"); 
		printf("||                  3.Hien thi danh sach phieu muon    ||\n"); 	
		printf("||                  4.Quay tro lai                     ||\n"); 
		printf("=========================================================\n"); 
		enter = getNumber("Nhap lua chon",1,6); 
		switch(enter){
			case 1 : { 
				addBorrow(); 
				break;
			} 
			case 2 : { 
				returnBook() ; 
				break;
			}
			case 3: { 
				printBorrowList(); 
				break;
			}
			case 4 : { 
				return ; 
			}
			default : { 
				printf("Vui long lua chon lai!!!!"); 
				break;
			}
		}
	}while(enter!= 4); 
}

void printMenu(){ 
	int enter  ;
	do{ 
		printf("\n======================THU VIEN======================\n"); 
		printf("||                1.Nhap sach                     ||\n"); 
		printf("||                2.Muon sach                     ||\n"); 
		printf("||                3.Thoat                         ||\n");
		printf("====================================================\n"); 
		enter = getNumber("Nhap lua chon",1,6); 
		switch(enter){ 
			case 1 : { 
				printBook(); 
				break;
			}
			case 2 : { 
				printBorrow(); 
				break;
			}
			case 3 : { 
				printf("Ban da thoat chuong trinh thanh cong !!! TAM BIET !!!"); 
				break;
			}
			default : { 
				printf("Vui long nhap lai lua chon !!!!\n"); 
				break;
			}				
		}
	}while(enter!=3); 
}


int main(){ 
	printMenu(); 
	return 0 ; 
}

