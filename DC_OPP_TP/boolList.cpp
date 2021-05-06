
/* 	=======================================
	[리스트 메소드 정의용 소스파일}
	  리스트 내 복잡한 메소드를 여기서 정의
	======================================= */

#include "boolList.hpp"

/* Lists 클래스 내 함수 */
bool Lists::findBoolty(char* txt){ // 해당 bool Eq가 존재하는지 찾기
    booltype *currBt = head;
    while(currBt != NULL){
        int samebits = 0;
        for (int i = 0; i < bits; ++i){
            if (currBt->boolvalue[i] == txt[i]) {++samebits;}
        }
        if (samebits == bits){return 1;}
    }
    return 0;
}
ImplicantList* Lists::newLists(){ // 후에 연결되는 리스트 추가
    ImplicantList* newImpli = new ImplicantList(index+1, this);
    booltype *currIBt = gethead();
    while(currIBt != NULL){
        booltype *cmpIBt = getOneStart(currIBt->one + 1);
        int idx = cmpIBt->one;
        if (cmpIBt == NULL){break;}
        while(idx == cmpIBt->one){
            int check = -1;
            for(int i = 0; i < bits; ++i){
                if (currIBt->boolvalue[i] != cmpIBt->boolvalue[i]){
                    if (check != -1){check = -1; break;}
                    check = i;
                }
            }
            if (check != -1){ // 추가
                newImpli->insertImpliEq(currIBt->boolvalue, check);
                currIBt->connect = true;
                cmpIBt->connect = true;
            }
            cmpIBt = cmpIBt->next;
        }
    }
    if (newImpli->gethead() == NULL){
        return NULL;
    }
    return newImpli;
}
booltype* Lists::getOneStart(int idx){ // one을 idx로 하는 최초 booltype 구하기
    booltype *currImp = gethead();
    while(currImp != NULL){
        if (currImp->one == idx){return currImp;}
    }
    return NULL;
}

/* boolList 클래스 내 함수 */
void boolList::insertBoolEq(char* intxt){ // 초기 Bool Eq를 추가하는 함수
	// don't care / minterm 구분과 함께 booltype 객체 생성
	booltype* newEq;
	if (intxt[0] == 'd'){
		newEq = new dontcare(intxt + 2);
	}
	else if (intxt[0] == 'm'){
		newEq = new minterm(intxt + 2);
	}
	// 1의 갯수에 맞게 올바른 위치에 삽입
		// 일단 head가 NULL이면 그냥 바로 삽입
		if (head == NULL){
			head = newEq;
			return;
		}

		// head가 존재하면 1의 갯수에 맞게 추가
		booltype* currEq = head, *lastEq = NULL;
		while (currEq != NULL){
			if (currEq->one == newEq->one){
				newEq->next = currEq->next;
				currEq->next = newEq;
				return;
			}
			else if (currEq->one >= newEq->one){
				newEq->next = currEq;
				if (lastEq == NULL){head = newEq; return;}
				lastEq->next = newEq;
				return;
			}
			lastEq = currEq;
			currEq = currEq->next;
		}
		lastEq->next = newEq;
		return;
}

/* ImlicantList 클래스 내 함수 */
void ImplicantList::insertImpliEq(char* intxt, int baridx){ // implicant를 추가하는 함수

    // 중복 확인
    char* newtxt = new char[bits+1];
    for (int i = 0; i < bits; ++i){ 
        if (i == baridx) {newtxt[i] = '-'; continue;}
        newtxt[i] = intxt[i];
    } newtxt[bits] = '\0'
    if (findBoolty(newtxt) == 1) {delete[] newtxt; return;}

	booltype* newEq = new booltype(newtxt);
    delete[] newtxt;
	// 1의 갯수에 맞게 올바른 위치에 삽입
		// 일단 head가 NULL이면 그냥 바로 삽입
		if (head == NULL){
			head = newEq;
			return;
		}

		// head가 존재하면 1의 갯수에 맞게 추가
		booltype* currEq = head, *lastEq = NULL, *sameOneEq = NULL;
		while (currEq != NULL){
			if (currEq->one == newEq->one){
				int sameBit = 0;
				sameOneEq = currEq;
				for(int i = 0; i < bits; ++i){
					if (newEq->boolvalue[i] == currEq->boolvalue[i]){++sameBit;}
				}
				if (sameBit == bits){newEq->~booltype(); return;}
			}
			else if (currEq->one >= newEq->one){
				newEq->next = currEq;
				if (lastEq == NULL){head = newEq; return;}
				lastEq->next = newEq;
				return;
			}
			lastEq = currEq;
			currEq = currEq->next;
		}
		if (sameOneEq != NULL){newEq->next = currEq->next; currEq->next = newEq;}
		lastEq->next = newEq;
		return;
}