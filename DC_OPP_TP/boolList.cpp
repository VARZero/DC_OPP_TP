/* 	=======================================
	[리스트 메소드 정의용 소스파일}
	  리스트 내 복잡한 메소드를 여기서 정의
	======================================= */

#include "boolList.hpp"

/* Lists 클래스 내 함수 */
bool Lists::findBoolty(char* txt){ // 해당 bool Eq가 존재하는지 찾기
	// 동일하면 1을 반환, 틀리면 0을 반환
    booltype *currBt = head;
    while(currBt != NULL){
        int samebits = 0;
        for (int i = 0; i < bits; ++i){
			// 한글자씩 비교
            if (currBt->boolvalue[i] == txt[i]) {++samebits;}
        }
        if (samebits == bits){return 1;} // 내부의 비트가 총 비트와 동일하면 1을 반환
		currBt = currBt->next;
    }
    return 0; // 내부 비트 모두 돌아봤을때 없다면 0을 반환
}
ImplicantList* Lists::newLists(){ // 후에 연결되는 리스트 추가
    ImplicantList* newImpli = new ImplicantList(index+1, this);
    booltype *currIBt = gethead();
    while(currIBt != NULL){
        booltype *cmpIBt = getOneStart(currIBt->one + 1);
        int idx = cmpIBt->one;
        if (cmpIBt == NULL){break;}
		while (cmpIBt != NULL) {
			if (idx == cmpIBt->one) {
				int check = -1;
				for (int i = 0; i < bits; ++i) {
					if (currIBt->boolvalue[i] != cmpIBt->boolvalue[i]) {
						if (check != -1) { check = -1; break; }
						check = i;
					}
				}
				if (check != -1) { // 추가
					newImpli->insertImpliEq(currIBt->boolvalue, check);
					currIBt->connect = true;
					cmpIBt->connect = true;
				}
				cmpIBt = cmpIBt->next;
			}
			else { break; }
		}
		currIBt = currIBt->next;
		if (currIBt->one == bits - index) { break; }
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
        currImp = currImp->next;
    }
    return NULL;
}
void Lists::getUnconnect(PIList* PI){ // unconnect되는 모든 요소를 반환하는 함수
	// PIList 포인터 형태로 반환
    booltype* currBt = head;
    while(currBt != NULL){
        if (currBt->connect == false){
            PI->insertPI(currBt->boolvalue);
        }
        currBt = currBt->next;
    }
}

/* boolList 클래스 내 함수 */
void boolList::insertBoolEq(char* intxt){ // 초기 Bool Eq를 추가하는 함수
	// don't care / minterm 구분과 함께 booltype 객체 생성
	booltype* newEq = NULL;
	if (intxt[0] == 'd'){
		newEq = new dontcare(intxt + 2);
	}
	else if (intxt[0] == 'm'){
		minterm* newMin = new minterm(intxt + 2);
		newEq = newMin;

		if (minhead == NULL) { minhead = newMin; }
		else { newMin->mtnext = minhead; minhead = newMin; }
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
    } newtxt[bits] = '\0';
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
		if (sameOneEq != NULL){newEq->next = sameOneEq->next; sameOneEq->next = newEq;}
		lastEq->next = newEq;
		return;
}

/* PIList 클래스 내 메소드 */
void PIList::insertPI(char* txt){ // Prime Implicant 추가
    booltype* newPi = new booltype(txt);
    if (head == NULL){
        head = newPi;
    }
    else {
        newPi->next = head;
        head = newPi;
    }
}
booltype* PIList::gethead(){ // head 출력
    return head;
}
void PIList::getEPI(PIList* EPIs, boolList* minterms, int *count) { // Essential Prime Implicant 구하기
	minterm* currMin = minterms->getMinhead();
	
	while (currMin != NULL) {
		booltype* currPI = head;
		booltype* cover = NULL;
		while (currPI != NULL) {
			int boolsame = 0;
			for (int i = 0; i < bits; ++i) {
				if (currMin->boolvalue[i] == currPI->boolvalue[i] || currPI->boolvalue[i] == '-') {
					++boolsame;
				}
				else { break; }
			}
			if (boolsame == bits) {
				if (cover != NULL) { cover = NULL; break; }
				cover = currPI;
			}
			currPI = currPI->next;
		}
		if (cover != NULL) {
			EPIs->insertPI(cover->boolvalue);
			++(*count);
		}
		currMin = currMin->mtnext;
	}
}