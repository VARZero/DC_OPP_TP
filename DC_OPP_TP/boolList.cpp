/* 	=======================================
	[리스트 메소드 정의용 소스파일}
	  리스트 내 복잡한 메소드를 여기서 정의
	======================================= */

#include "boolList.hpp"

/* Lists 클래스 내 함수 */
Lists* Lists::newLists(){
    ImplicantList* newImpli = new ImplicantList(index+1, this);
    
    if (newImpli->gethead() == NULL){
        return NULL;
    }
    return newImpli;
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
void ImplicantList::insertImpliEq(char* intxt){ // implicant를 추가하는 함수
	booltype* newEq = new booltype(intxt);
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
void ImplicantList::checkPrevConnect(){// 이전 리스트에서 Prime Implicant를 찾기위해 연결되지 않는것을 찾는것
	booltype *currIBt = head /* 이 리스트의 Implicant 식 */,
		*currCBt = prevList->gethead(); /* 이전에 연결된 리스트의  */
	while (currIBt != NULL){
		while (currCBt != NULL){ // 끝이 아닌지 확인
			if (currCBt->one == currIBt->one ||
			 currCBt->one == currIBt->one +1){ // 이웃하는 경우인지 확인
				for (int i = 0; i < bits; ++i){
					if (currIBt->boolvalue[i] != currCBt->boolvalue[i] || 
					 currCBt->boolvalue[i] != '_'){ // 일치하지 않는 경우를 찾을때
						currCBt->connect = false;
					}
				}
				currCBt = currCBt->next;
			} 
			else{break;}
		}
	    currIBt = currIBt->next;
	}
}