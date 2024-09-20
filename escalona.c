#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>

#define NUM_TRANSAS 1000
#define NUM_NODOS 1000
#define NUM_OPERS 1000
#define MAX_FILHOS 1000
//============================
//EXECUTAR COM ./escalona < in 
//============================


//				TRANSACAO
// tempo id_transacao operacao atributoAlvo
//     1         1       R          X

int transasAtivas = 0;
int agendamento_id =1;
int idUlt;



typedef struct Operacao {
	int id;
	char operacao;
	char atributoAlvo;
	int tempo;

} Operacao;


typedef struct Nodo{
	struct Nodo ** filhos;
	int numFilhos;
	char estado;
	int id;

	Operacao * operList;
	int numOper;
} Nodo;


typedef struct Agendamento{

	Operacao * operList; //Operacaos do agendamento;
	Nodo * nodoList; //nodos do agendamento
	int numNodos;
	int numOper;

} Agendamento;


void newAgendamento(Agendamento * a) {

	a->operList = (Operacao *) malloc (NUM_OPERS*sizeof(Operacao));
	a->nodoList =(Nodo *) malloc (NUM_NODOS*sizeof(Nodo));
	a->numNodos = 0;
	a->numOper = 0;

}






//adiciona Operacao ao agendamento
void newOper (Agendamento * a, int tempo,int id, char op, char atributoAlvo) {

	Operacao * oper = &a->operList[a->numOper];
	oper->atributoAlvo=atributoAlvo;
    oper->operacao = op;
    oper->id = id;
    oper->tempo = tempo;

    a->numOper++;

}



//adiciona transacao ao agendamento
void newNodo (Agendamento * a, int id) {
	
	
	//instancia um novo nodo
	Nodo * nodo = &(a->nodoList[a->numNodos]);

    nodo->id = id;
	
	nodo->numFilhos = 0;
	nodo->estado = 0; //para a busca em profundidade
	nodo->filhos = (Nodo **) malloc(MAX_FILHOS*sizeof(Nodo *));
	transasAtivas++; //var global de transacoes ativas
	a->numNodos++;

}

int existeTransa(Agendamento * a, int id){
	int i;
	for(i=0;i<a->numNodos;++i){
		if (a->nodoList[i].id == id) return 1;
	}
	return 0;
}



void printAgendamento(Agendamento * a) {
	int i,j;
	Operacao * oper;
	for (i = 0; i < a->numOper; ++i) {
		oper = &a->operList[i];
		printf("%d %d %c %c\n",oper->tempo,oper->id,oper->operacao,oper->atributoAlvo);
	}

}








void equivalencia(Agendamento * a) {
	int i;

	for(i=0; i < a->numNodos; ++i) {
		//TODO faz as combinacoes e checa as 3 regras:
/*
	    O mesmo conjunto de transações e operações participam em S e S'
	    Para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
	    Se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S'
*/
	}


}

Nodo * getNodeById(Agendamento * a, int id) {
	int i;
	for (i=0;i < a->numNodos; ++i) {
		if(a->nodoList[i].id == id) {
			return &a->nodoList[i];
		}
	}
	return NULL;
}

/*
 1 R X   2 R X 
 1 W X   2 W X
*/

void getOpByNode (Agendamento * a,Nodo * n) {

	n->operList =(Operacao *) malloc(NUM_OPERS*sizeof(Operacao));
	int i;
	for(i = 0; i < a->numOper; ++i) {
		if(a->operList[i].id == n->id ) {
			n->operList[n->numOper] = a->operList[i];
			n->numOper++;
		}
	}
}


void seriabilidade(Agendamento * a) {
    int i,j,k,achou;
    int iOP,jOP;
    /*
    Crie um nó para cada T do escalonamento S
    Aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
    Aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti
    Aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti
    S é serial se não existe ciclo no grafo
    */
     
    for(i = 0; i< a->numNodos;++i){

        for(j = 0; j < a->numNodos;++j){

        	if(a->nodoList[i].id == a->nodoList[j].id) continue;
			 Nodo *nodoI = &a->nodoList[i];
			 Nodo *nodoJ = &a->nodoList[j];        
   
        	for(iOP = 0; iOP < nodoI->numOper ;++iOP){
        		for(jOP = iOP; jOP < nodoJ->numOper ;++jOP){
        			     

        			Operacao *opI = &nodoI->operList[iOP]; 
        			Operacao *opJ = &nodoJ->operList[jOP]; 
        			int existe = 0;

        			int z;
        			for (z=0;z < nodoI->numFilhos; ++z) {
						if(nodoI->id ==nodoI->filhos[z]->id) {
							existe = 1;			
						}
					}


					if (existe) continue;
        			if(opI->operacao == 'R' && opJ->operacao == 'W' && opI->atributoAlvo == opJ->atributoAlvo && opI->tempo < opJ->tempo){

	              //  	printf("Tipo 2\n");
	                	
	            		nodoI->filhos[nodoI->numFilhos] = nodoJ;
	                	nodoI->numFilhos++;

                	
	                }
	                else if(opI->operacao == 'W' && opJ->operacao == 'R' && opI->atributoAlvo == opJ->atributoAlvo && opI->tempo < opJ->tempo ){
	                //	printf("Tipo 1\n");
	                    
	            		nodoI->filhos[nodoI->numFilhos] = nodoJ;
	                	nodoI->numFilhos++;
	                	

	                }
	                else if(opI->operacao == 'W' && opJ->operacao == 'W' && opI->atributoAlvo == opJ->atributoAlvo && opI->tempo < opJ->tempo ){
	           		//	printf("Tipo 3\n");
	               
	            		nodoI->filhos[nodoI->numFilhos] = nodoJ;
	                	nodoI->numFilhos++;
	                	

	                }

        		}

        	}
        	
        }


    }       
    

     
    //busca em largura
    achou = 0;									//estado 0 = branco
    											//estado 1 = cinza
    											//estado 2 = preto



    a->nodoList[i].estado = 0;
    int sizef = 0;
    int atual = 0;


 
    Nodo **fila =(Nodo **) malloc(NUM_NODOS * sizeof(Nodo *)); //declara a fila
    for(i = 0; i < a->numNodos && !achou;++i){  
         
    	if(a->nodoList->estado == 0) {
    	  fila[sizef] =  &a->nodoList[i]; //insere o nodo
		  ++sizef;
    	}


    	while(atual < sizef) {


    		for (j = 0; j<fila[atual]->numFilhos; ++j) { //percorre todos os filhos do no corrente
    			fila[atual]->estado = 1;

    			if(fila[atual]->filhos[j]->estado == 0) {
    				
                	fila[sizef] = fila[atual]->filhos[j];
                	sizef++;
                //	printf("oie\n");
                	
    			}

    			
    			else {
    				
    					
    				for(k = 0; k <fila[atual]->filhos[j]->numFilhos;++k) {
    				//	printf("|%d <-> %d|\n",fila[k]->id,fila[atual]->filhos[j]->filhos[k]->id);
    					if(fila[atual]->id == fila[atual]->filhos[j]->filhos[k]->id) {
    					//	printf("|%d <-> %d|\n",fila[k]->id,fila[atual]->filhos[j]->id );
    						printf("NS ");
    						return;
    					}
    				}
    				
    			}
		
    		}
    		fila[atual]->estado = 2;
    		++atual;
    		    		

    	}
    



    	

         /*
         for(atual = 0; atual < sizef && !achou;++atual) {
         	  	
             for(j = 0; j<fila[atual].numFilhos&& achou == 0;++j){
             		
                for(k = 0; k<sizef && achou == 0;++k){

                	printf("|%d <-> %d|\n",fila[k].id,fila[atual].filhos[j].id );
                    
                    if(fila[k].id == fila[atual].filhos[j].id){
                        achou = 1;
                    }
                }
                
             }
         }
         */
    }

    /*
    for(i = 0; i < a->numNodos && !achou;++i){  
         Nodo *fila =(Nodo *) malloc(NUM_NODOS * sizeof(Nodo)); //declara a fila
//       fila[0] = a->nodoList[i];
         int sizef = 0;
         int atual;
         
		 fila[sizef] =  a->nodoList[i]; //insere o ponto
		 ++sizef;
         
         for(atual = 0; atual < sizef && !achou;++atual) {
         	  	
             for(j = 0; j<fila[atual].numFilhos&& achou == 0;++j){
             		
                for(k = 0; k<sizef && achou == 0;++k){

                	printf("|%d <-> %d|\n",fila[k].id,fila[atual].filhos[j].id );
                    
                    if(fila[k].id == fila[atual].filhos[j].id){
                        achou = 1;
                    }
                }
                sizef++;
                fila[sizef] = fila[atual].filhos[j];
             }
         }
    }
    */
   
  
    printf("SS ");
   


}


void swapInt(int a[], int x,int y){

    int temp;
    temp = a[x];
    a[x] = a[y];
    a[y] = temp;

}

// se retorno = 1, NV, senão SV
int visaoSegunda(Agendamento *a,int * vetorComb) {
	int i,j,k;
	
	int stop = 0;
	


	int att = 0;


	Nodo * vetorTrans; 
	vetorTrans = (Nodo *) malloc (NUM_NODOS * sizeof(Nodo));
	
	Operacao *vetorOper = (Operacao *) malloc (NUM_OPERS * sizeof(Operacao));

	vetorTrans[a->numNodos-1] = a->nodoList[idUlt]; 
//	printf("Ultimo W: %d\n",idUlt);
	/*
	for(i = 0; i< a->numNodos; i++) {
		
	    for(j = 0; j<vetorTrans[i].numOper; ++j) {
	    	printf("!!!");
	        vetorOper[att] = vetorTrans->operList[j];
	        ++att;
	    }
	}
	*/

	for(i = 0; i< a->numNodos; i++) {
		att += a->nodoList[i].numOper;
	}



	int existsW = 0;
	int ii,jj;
	int corretude = 1; //supoe que esta correto


	if(att == a->numOper) { //numero de operacoes da visao eh igual ao do agendamento REGRA 1

		for (i = 0;i < a->numNodos-1 && corretude;++i) { //percorre todas as transacoes permutadas

			Nodo * currentWriteNode = getNodeById(a,vetorComb[i]); 
			//printf("eu sou %d\n",currentNode->id);
			for (j = 0; j < currentWriteNode->numOper && corretude;++j) {

				int ordemR = 0;
				if(currentWriteNode->operList[j].operacao == 'W') { //encontrou uma escrita
					existsW = 1;
				//	printf("Achei W: %d\n",currentWriteNode->id);

					for (ii = 0;ii < a->numNodos-1 && corretude;++ii) { //percorre todas as operacoes permutadas
						Nodo * currentReadNode = getNodeById(a,vetorComb[ii]); 
						for (jj = 0; jj < currentReadNode->numOper && corretude;++jj) {
							if(currentReadNode->operList[jj].operacao == 'R' //se o R se refere ao mesmo W encontrado
								&& currentReadNode->operList[jj].atributoAlvo == currentReadNode->operList[j].atributoAlvo) {

								++ordemR;
							//	printf("Achei R(%c): %d\n",currentReadNode->operList[jj].atributoAlvo,currentReadNode->id);
								int ordemRchk = 0;
								for(k=0; k < a->numOper && corretude;++k){ //percorro lista original do agendamento para checar a ordem
									
									if (a->operList[k].operacao == 'R' 
										&& 	a->operList[k].atributoAlvo == currentReadNode->operList[jj].atributoAlvo
										&&  a->operList[k].id != currentReadNode->operList[jj].id) {
										ordemRchk++;
									//	printf("%d %d\n",ordemRchk, ordemR);
   
										if(ordemRchk == ordemR) {
											if(a->operList[k].tempo > currentReadNode->operList[jj].tempo) {
											//	printf("Ordem Correta!\n") ;
											}

											else {
									//			printf("Ordem Incorreta!\n");
												corretude=0;
											}
										}
									}
									

									
								}
								
							}
							//printf("!!");
						}
					}
				//	printf("===Last!===\n");
										
					Nodo * currentReadNode = getNodeById(a,idUlt);
					for (jj = 0; jj < currentReadNode->numOper && corretude ;++jj) { //LACO DA ULTIMA TRANSACAO
							if(currentReadNode->operList[jj].operacao == 'R' //se o R se refere ao mesmo W encontrado
								&& currentReadNode->operList[jj].atributoAlvo == currentReadNode->operList[j].atributoAlvo) {
						//		printf("Achei R(%c): %d\n",currentReadNode->operList[jj].atributoAlvo,currentReadNode->id);

								int ordemRchk = 0;
								for(k=0; k < a->numOper && corretude ;++k){ //percorro lista original do agendamento para checar a ordem
									
									if (a->operList[k].operacao == 'R' 
										&& 
										a->operList[k].atributoAlvo == currentReadNode->operList[jj].atributoAlvo) {
										ordemRchk++;
									//	printf("%d %d\n",ordemRchk, ordemR);
   
										if(ordemRchk == ordemR) {
											if(a->operList[k].tempo > currentReadNode->operList[jj].tempo) {
											//	printf("Ordem Correta!:");
											}

											else {
										//		printf("Ordem Incorreta!");
												corretude=0;
											} 
										}
									}
									

									
								}
								
							}
							//printf("!!");
						}
				//	printf("===Last!===\n");
					



				}
			}
		}


		/*for(i = att-1; i>0; --i) {
			if(vetorOper[i].operacao == 'W') { //encontrou uma escrita

			}
		}


*/
	    /*
	    for(i = att-1; i>0 && k != 1; --i) { //comeca da penultima transacao
	        k = 0;

	        if(vetorOper[i].operacao == 'R') { //i está depois de R
	            for(j = 0; j<a->numOper && k != 1; ++j) {
	                if(vetorOper[j].operacao == 'W' && vetorOper[j].id != vetorOper[i].id) {
	                    if(( (vetorOper[j].tempo < vetorOper[i].tempo)&&( i < j))) k = 1;
	                }
	            }
	        }

	    }
	    */
	}



	if (existsW == 0 || corretude == 1) return 0;//nao ha escritas alem de W e
												 //a ordem das escritas de R(x) está correta
	return 1; //retorno 1 = nao equivalente




	//for(i=0; i < a->numNodos; ++i) {
	//TODO faz as combinacoes e checa as 3 regras:
	// O mesmo conjunto de transações e operações participam em S e S' Para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o
	// mesmo deve permanecer para r(x) de Ti em S' Se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S' 
}

int permute(Agendamento * agen, int * a, int l, int r) {
	//retorno 1 = nao equivalente
   int i;
 //  printf("%d %d\n",l,r);
   
   if (l == r){
//   		printf("!!!");
   	  for (i = 0; i < agen->numNodos-1; ++i) { //deixa de lado a ultima escrita;
   	 // 	printf("Permutacao: %d ",a[i]);
   	  }

     if(!visaoSegunda(agen,a)) return 0;  //se houver visao serializavel
  //   printf("\n");
    }
   else
   {
       for (i = l; i <= r; i++)
       {
          //swap((a+l), (a+i));
          swapInt(a,l,i);
          if (permute(agen,a, l+1, r)) return 0 ; 
          swapInt(a,l,i);
          //swap((a+l), (a+i)); //backtrack
       }
   }

   return 1;
}

void visao(Agendamento *a) {
	/*
	    O mesmo conjunto de transações e operações participam em S e S'
	    Para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
	    Se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S'
*/

	int i,j,k;
	idUlt = -1;
	int stop = 0;
	for( i = a->numOper-1; i>0 && stop == 0; --i) {//regra do ultimo w
	    if(a->operList[i].operacao == 'W') //obtem a ultima escrita
	    {
	        
	        idUlt = a->operList[i].id;
	        break;
	    }
	}
	
	int * vetorComb =(int *) malloc(sizeof(int) * a->numNodos-1);

	int atual = 0;
	for(i = 0; i<a->numNodos; ++i){ //monta vetor pra sofrer arranjo ft. mixeu
	    if(a->nodoList[i].id != idUlt){ //monta o vetor que será combinado
	        vetorComb[atual] = a->nodoList[i].id;
	        atual++;
	    }
	}

	


	//printf("VRAU*************\n");	//retorno 1 = nao equivalente
	if (permute(a,vetorComb,0,a->numNodos-2)){
		printf("NV\n");
	    //não equivalente
	}

	else {
		printf("SV\n");

	}
	//tudo aqui embaixo tem que ficar dentro do loop que monta os arranjos 

		
}	





void main () {

	int tempo, id_transacao;
	char op, atributoAlvo;
	int tempoAnterior;
	Agendamento agenda;

	newAgendamento(&agenda);

	struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
   
    
	for (;;) {

		
       
		if( poll(&mypoll, 1, 1000) )    {
			 scanf("%d %d %c %c",&tempo,&id_transacao,&op,&atributoAlvo); //le transacao por transacao
			 if(tempoAnterior == tempo) {
			 	return;
			 }
		}

		else {
	
			return;
		}
		
		tempoAnterior = tempo;

		if(op == 'C') { //se for um commit
			transasAtivas--;

		}

		else { //se não


			if(existeTransa(&agenda,id_transacao)) { //se o id está no agendamento
				
				newOper(&agenda,tempo,id_transacao,op,atributoAlvo);
			}

			else { //não existe essa transacao
				newNodo(&agenda,id_transacao);
				newOper(&agenda,tempo,id_transacao,op,atributoAlvo);
			}
		}


		//todos os commits foram feitos
		if(transasAtivas == 0) { //cria novo agendamento;

			//printAgendamento(&agenda);

			int i;

			printf("%d ",agendamento_id);
			for(i=0; i < agenda.numNodos; ++i) {
				//obtem todas as operacoes de cada nodo
				if (i < agenda.numNodos-1) printf("%d,",agenda.nodoList[i].id);
				else printf("%d ",agenda.nodoList[i].id);
				getOpByNode(&agenda,&agenda.nodoList[i]);
				//nao, num usei a lista encadeada (let it gol :2)
			}



			seriabilidade(&agenda);
			visao(&agenda);

			newAgendamento(&agenda); //comeca um novo ciclo
			++agendamento_id;
		}
   
    }   
    



}
