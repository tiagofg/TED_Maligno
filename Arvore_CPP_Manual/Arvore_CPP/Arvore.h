#pragma once
#include "NoArvore.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include "Pilha.h"

using namespace std;
#define GIRO_PARA_ESQUERDA 0
#define GIRO_PARA_DIREITA 1
#define TIPO_STRUCT_TOSTRING 1
#define TIPO_STRUCT_INCLUIRRECUR 2
#define TIPO_STRUCT_INCLUIRSUBARVORE 3
#define TIPO_STRUCT_EXCLUIRSUBARVORE 4
#define TIPO_STRUCT_VERIFICABALANCEAMENTO 5
#define TIPO_STRUCT_ACHARNOERRONEO 6
#define TIPO_STRUCT_EXCLUIRRECUR 7
#define TIPO_STRUCT_MAIORNIVELRECUR 8

template <class Tipo>
class Arvore
{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// construtor nulo
		Arvore() : raiz(nullptr){}

		// construtor de c�pia
		Arvore(const Arvore<Tipo> &original) : setRaiz(original.getRaiz()) {}

		// construtor com 1 dado inicial
		Arvore(const Tipo &novoDado) : setRaiz(novoDado) {}

		// destrutor
		~Arvore() {}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// getter da raiz para saber qual a raiz da �rvore diretamente
		Tipo getRaiz() const
		{
			return this->raiz->getDado();
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------M�TODOS PRINCIPAIS----------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	
		// m�todo excluir um dado
		bool excluir(const Tipo &dado)
		{
			// se for a raiz que se deseja excluir
			if (this->raiz == nullptr)
			{
				cout << "BOT> A lista ja esta vazia..." << endl;
				return false;
			}
			bool ret = false;
			if (this->raiz->getDado() == dado)
			{
				// ponteiro auxiliar para podermos
				NoArvore<Tipo>* atual;
				ret = true;
				// menor dos maiores para quando s� a direita � nulo ou quando nenhum � nulo
				if ((raiz->getDireita() == nullptr && raiz->getEsquerda() != nullptr) || 
					(raiz->getDireita() != nullptr && raiz->getEsquerda() != nullptr))
				{
					atual = raiz->getEsquerda();
					while (atual->getDireita() != nullptr)
						atual = atual->getDireita();
					Tipo novoDadoRaiz = atual->getDado();
					this->excluirRecur(novoDadoRaiz, raiz);
					this->raiz->setDado(novoDadoRaiz);
				}
				// maior dos menores se somente a esquerda � nula
				else if (raiz->getDireita() != nullptr && raiz->getEsquerda() == nullptr)
				{
					atual = raiz->getDireita();
					while (atual->getEsquerda() != nullptr)
						atual = atual->getEsquerda();
					Tipo novoDadoRaiz = atual->getDado();
					this->excluirRecur(novoDadoRaiz, raiz);
					this->raiz->setDado(novoDadoRaiz);
				}
				// quando a raiz � folha
				else
					this->raiz = nullptr;
			}
			else
			{
				// n�o � raiz
				ret = this->excluirRecur(dado, this->raiz);
			}
			// falamos que excluiu
			if (ret != false)
			{
				cout << "BOT> Excluido o " << dado << endl;
				// balancear
				this->balancear(1, dado);
			}
			else
				cout << "BOT> Nao existe o valor " << dado << " na lista" << endl;
			return ret;
		}

		void incluir(const Tipo &novoDado)
		{
			// se n�o tiver nem raiz, inclu�mos a raiz
			if (this->raiz == nullptr)
			{
				this->raiz = new NoArvore<Tipo>(novoDado);
				cout << "BOT> Incluido na raiz o " << novoDado << endl;
				return;
			}
			// mostramos que inclu�mos
			cout << "BOT> Incluido o " << novoDado << endl;
			// inclu�mos
			this->incluirRecur(novoDado, raiz);
			// balanceamos
			this->balancear(0, novoDado);
		}

		friend ostream& operator<< (ostream &os, Arvore<Tipo> &aArvore)
		{
			// escrevemos o valor da �rvore
			return (os << "TREE> " << aArvore.toString());
		}

		string toString()
		{
			// retornamos o conte�do da �rvore desde a raiz
			return this->toString(this->raiz);
		}
		friend class Pilha<Tipo>;
	protected:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//--------------------------------------------------------------------STRUCTS----------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		struct IEmpilhavel 
		{
			unsigned int indicacao;
			unsigned int tipo;
			virtual ~IEmpilhavel() {};
			virtual IEmpilhavel* clone()
			{
				IEmpilhavel* ret = new IEmpilhavel();
				ret->indicacao = this->indicacao;
				ret->tipo = this->tipo;
				return ret;
			};
		};
		struct structToString : public IEmpilhavel
		{
			NoArvore<Tipo>* no;
			string noEsquerda;
			string noDireita;
			structToString()
			{
				tipo = TIPO_STRUCT_TOSTRING;
			}
			structToString* clone()
			{
				return new structToString(this->no, this->noEsquerda, this->noDireita, indicacao);
			}
		protected:
			structToString(const NoArvore<Tipo>* novoNo, const string &noEsquerda, const string &noDireita, const int &indicacaoNova) : noEsquerda(noEsquerda), noDireita(noDireita)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_TOSTRING;
				indicacao = indicacaoNova;
			}
		};

		struct structIncluirRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structIncluirRecur() {
				tipo = TIPO_STRUCT_INCLUIRRECUR;
			}
			structIncluirRecur* clone()
			{
				return new structIncluirRecur(no);
			}
		protected:
			structIncluirRecur(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_INCLUIRRECUR;
			}
		};

		typedef struct structIncluirSubArvore : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structIncluirSubArvore() {
				tipo = TIPO_STRUCT_INCLUIRSUBARVORE;
			}
			structIncluirSubArvore* clone()
			{
				return new structIncluirSubArvore(no);
			}
		protected:
			structIncluirSubArvore(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_INCLUIRSUBARVORE;
			}
		};

		struct structExcluirSubArvore : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structExcluirSubArvore() {
				tipo = TIPO_STRUCT_EXCLUIRSUBARVORE;
			}
			structExcluirSubArvore* clone()
			{
				return new structExcluirSubArvore(this->no);
			}
		protected:
			structExcluirSubArvore(const NoArvore<Tipo> *novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_EXCLUIRSUBARVORE;
			}
		};

		typedef struct structVerificaBalanceamento : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structVerificaBalanceamento() {
				tipo = TIPO_STRUCT_VERIFICABALANCEAMENTO;
			}
			structVerificaBalanceamento* clone()
			{
				return new structVerificaBalanceamento(this->no);
			}
		protected:
			structVerificaBalanceamento(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
			}
		};

		struct structAcharNoErroneo : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			NoArvore<Tipo>* achado;
			structAcharNoErroneo() {
				tipo = TIPO_STRUCT_ACHARNOERRONEO;
			}
			structAcharNoErroneo* clone()
			{
				return new structAcharNoErroneo(no, achado);
			}
		protected:
			structAcharNoErroneo(const NoArvore<Tipo> *novoNo, const NoArvore<Tipo>* achado)
			{
				this->no = new NoArvore<Tipo>(novoNo);
				this->achado = new NoArvore<Tipo>(achado);
				tipo = TIPO_STRUCT_ACHARNOERRONEO;
			}
		};

		typedef struct structExcluirRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structExcluirRecur() {
				tipo = 7;
			}
		}structExcluirRecur;

		typedef struct structMaiorNivelRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			int aux;
			structMaiorNivelRecur() {
				tipo = TIPO_STRUCT_MAIORNIVELRECUR;
			}
			structMaiorNivelRecur* clone()
			{
				return new structMaiorNivelRecur(this->no);
			}
		protected:
			structMaiorNivelRecur(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//------------------------------------------------------------------METODOS AUXILIARES-------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		
		// m�todo recursivo que coloca os da esquerda e os da direita e o pr�prio
		string toString(const NoArvore<Tipo> *noParam) 
		{
			if (noParam == nullptr)
				return "";
			string noEsquerda = "", noDireita = "", retorno = "";
			structToString* aStruct = new structToString();
			NoArvore<Tipo> *no = new NoArvore<Tipo>(noParam);
			inicio:
			if (no->getEsquerda() != nullptr)
			{
				//noEsquerda = "(" + this->toString(no->getEsquerda()) + ")<-";
				aStruct->no = new NoArvore<Tipo>(no);
				aStruct->noEsquerda = noEsquerda;
				aStruct->noDireita = noDireita;
				aStruct->indicacao = 1;
				pilha.empilhar(aStruct);
				noEsquerda = "";
				noDireita = "";
				no = new NoArvore<Tipo>(no->getEsquerda());
				goto inicio;
			}
			retNoEsquerda:
			if (no->getDireita() != nullptr)
			{
				//noDireita = "(" + this->toString(no->getDireita()) + ")<-";
				aStruct->no = new NoArvore<Tipo>(no);
				aStruct->noEsquerda = noEsquerda;
				aStruct->noDireita = noDireita;
				aStruct->indicacao = 2;
				pilha.empilhar(aStruct);
				noEsquerda = "";
				noDireita = "";
				no = new NoArvore<Tipo>(no->getDireita());
				goto inicio;
			}
		retNoDireita:
			retorno = "";
			if (noEsquerda != "")
				retorno = "(" + noEsquerda + ")<-";
			retorno += "[" + to_string(no->getDado()) + "]";
			if (noDireita != "")
				retorno += "->(" + noDireita + ")";
			/// RECURS�O PRA TUDO QUANTO � LADO (s� vai na horizontal)
			// if (pilha.consegueDesempilharDoMesmoTipo())
			if (dynamic_cast<structToString*>(pilha.getTopo()))
			{
				bool paiDireto = (no->getDirecaoPai() == PAI_DIREITA);
				aStruct = dynamic_cast<structToString*>(pilha.desempilhar());
				no = aStruct->no;
				if (paiDireto)
				{
					noEsquerda = retorno;
					noDireita = aStruct->noDireita;
				}
				else
				{
					noDireita = retorno;
					noEsquerda = aStruct->noEsquerda;
				}
				if (aStruct->indicacao == 0)
					goto inicio;
				if (aStruct->indicacao == 1)
					goto retNoEsquerda;
				goto retNoDireita;
			}
			else
			{
				return retorno;
			}
		}

		// retorna o n� que poss�i o dado a ser achado
		NoArvore<Tipo>* getNoDoDado(int dadoAAchar)
		{
			NoArvore<Tipo>* no;
			no = this->raiz;
			while (no != nullptr)
			{
				if (dadoAAchar > no->getDado())
					no = no->getDireita();
				else if (dadoAAchar < no->getDado())
					no = no->getEsquerda();
				else // achou
					break;
			}
			return no;
		}

		// obt�m o menor dos maiores do n� dado
		NoArvore<Tipo>* getMenorDosMaiores(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return nullptr;

			NoArvore<Tipo>* aux = no->getDireita();

			if (aux == nullptr)
				return nullptr;

			while (aux->getEsquerda() != nullptr)
				aux = aux->getEsquerda();

			return aux;
		}

		// retorna o maior dos menores do n� dado
		NoArvore<Tipo>* getMaiorDosMenores(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return nullptr;

			NoArvore<Tipo>* aux = no->getEsquerda();

			if (aux == nullptr)
				return nullptr;

			while (aux->getDireita() != nullptr)
				aux = aux->getDireita();

			return aux;
		}

		void incluirRecur(const Tipo &novoDado, NoArvore<Tipo> *no)
		{
			structIncluirRecur* aStruct = new structIncluirRecur();
			inicio:
			if (novoDado > no->getDado())
			{
				if (no->getDireita() != nullptr)
				{
					/// RECURS�O DO NOH PARA DIREITA
					//incluirRecur(novoDado, no->getDireita());
					aStruct->no = no;
					pilha.empilhar(aStruct);
					no = no->getDireita();
					goto inicio;
				}
				else // if(direita � null)
				{
					no->setDireita(new NoArvore<Tipo>(novoDado));
					no->getDireita()->setPai(no);
				}
			}
			else if (novoDado < no->getDado())
			{
				if (no->getEsquerda() != nullptr)
				{
					/// RECURS�O DO NOH PARA ESQUERDA
					//incluirRecur(novoDado, no->getEsquerda());
					aStruct->no = no;
					pilha.empilhar(aStruct);
					no = no->getEsquerda();
					goto inicio;
				}
				else // if(esquerda � null)
				{
					no->setEsquerda(new NoArvore<Tipo>(novoDado));
					no->getEsquerda()->setPai(no);
				}
			}
			// FIM DE UMA RECURS�O
			while (true)
			{
				if (!dynamic_cast<structIncluirRecur*>(pilha.getTopo()))
					break;
				pilha.desempilhar();
			}
		}

		// excl�i recursivamente o n� com o dado desejado
		bool excluirRecur(const Tipo &dado, NoArvore<Tipo> *atual)
		{
			// n� inexistente
			if (atual == nullptr)
			{
				/// ACABANDO UMA RECURS�O
				return false;
			}

			if (dado == atual->getDado())
			{
				if (atual->isFolha()) // se � folha, s� excluir normal
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(nullptr);
					else
						atual->getPai()->setEsquerda(nullptr);
					atual = nullptr;
				}
				else if (atual->getEsquerda() == nullptr) // precisamos fazer quase que uma rota��o
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getDireita());
					else
						atual->getPai()->setEsquerda(atual->getDireita());
					atual->getDireita()->setPai(atual->getPai());
					atual = nullptr;
				}
				else if(atual->getDireita() == nullptr) // precisamos fazer quase que uma rota��o
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getEsquerda());
					else
						atual->getPai()->setEsquerda(atual->getEsquerda());
						atual->getEsquerda()->setPai(atual->getPai());
						atual = nullptr;
				}
				else // tem dois filhos, ent�o n�s colocamos qualquer um
				{
					if (getNivelMaiorDosMenores(atual) < getNivelMenorDosMaiores(atual))
					{
						NoArvore<Tipo> aux = *getMenorDosMaiores(atual);
						this->excluirRecur(aux.getDado(), this->raiz);
						atual->setDado(aux.getDado());
					}
					else
					{
						NoArvore<Tipo> aux = *getMaiorDosMenores(atual);
						this->excluirRecur(aux.getDado(), this->raiz);
						atual->setDado(aux.getDado());
					}
					atual = nullptr;
				}
				/// ACABANDO UMA RECURS�O
				return true;
			}
			else if (dado < atual->getDado())
			{
				/// RECURS�O DO NOH PARA ESQUERDA
				/// ACABANDO UMA RECURS�O
				return this->excluirRecur(dado, atual->getEsquerda());
			}
			else
			{
				/// RECURS�O DO NOH PARA DIREITA
				/// ACABANDO UMA RECURS�O
				return this->excluirRecur(dado, atual->getDireita());
			}
		}

		// m�todo recursivo que coloca no "maior" o valor do maior n� errado
		void maiorNivelRecur(NoArvore<Tipo>* no, int* maior)
		{
			if (no == nullptr)
				return;
			
			int aux = 1;
			structMaiorNivelRecur* aStruct = new structMaiorNivelRecur();

			inicio:
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				desempilha:
				if (dynamic_cast<structMaiorNivelRecur*>(pilha.getTopo()))
				{
					if (this->pilha.size() != 1)
					{
						// coloca no maior se o n�vel da folha for maior que o maior antigo
						if (aux > *maior)
							*maior = aux;

						aStruct = dynamic_cast<structMaiorNivelRecur*>(pilha.desempilhar());
						if (aStruct->no->getEsquerda() == no)
						{
							no = aStruct->no;
							aux = aStruct->aux;
							goto direita;
						}
						else if (aStruct->no->getDireita() == no)
						{
							no = aStruct->no;
							aux = aStruct->aux;
							goto esquerda;
						}
						else
						{
							no = aStruct->no;
							aux = aStruct->aux;
							goto direita;
						}
					}
					else
					{
						this->pilha.desempilhar();
						return;
					}
				}
				else
					return;
			}

			aStruct->no = no;
			aStruct->aux = aux;
			this->pilha.empilhar(aStruct);

			esquerda:
			if (no->getEsquerda() != nullptr)
			{
				no = no->getEsquerda();
				aux++;
				goto inicio;
			}
			
			direita:
			if (no->getDireita() != nullptr)
			{
				no = no->getDireita();
				aux++;
				goto inicio;
			}
		}

		// retorna a maior altura de um n�
		int maiorNivel(NoArvore<Tipo>* no)
		{
			int* maior = (int*)malloc(sizeof(int));
			*maior = 0;
			maiorNivelRecur(no, maior);
			return *maior;
		}

		// https://www.youtube.com/watch?v=zh27Tp8HV7E
		// verifica se a sub�rvore inteira est� balanceada
		bool verificaBalanceamento(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos balanceado o n�
			if (no == nullptr)
				return true;

			int countD = 0;
			int countE = 0;
			structVerificaBalanceamento* aStruct = new structVerificaBalanceamento();

			inicio:
			// se for folha, consideramos balanceado o n�
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				if (dynamic_cast<structVerificaBalanceamento*>(pilha.getTopo()))
				{
					if (this->pilha.size() != 1)
					{
						aStruct = dynamic_cast<structVerificaBalanceamento*>(pilha.desempilhar());
						if (aStruct->no->getEsquerda() == no)
						{
							no = aStruct->no;
							goto direita;
						}
						else if (aStruct->no->getDireita() == no)
						{
							no = aStruct->no;
							goto esquerda;
						}
						else
						{
							no = aStruct->no;
							goto direita;
						}
					}
					else
					{
						this->pilha.desempilhar();
						return true;
					}
				}
				else
					return true;
			}

			// retornamos se o n� est� balanceado, juntamente com os seus filhos
			if (abs(maiorNivel(no->getDireita()) - maiorNivel(no->getEsquerda())) < 2)
			{
				aStruct->no = no;
				this->pilha.empilhar(aStruct);

				esquerda:
				if (no->getEsquerda() != nullptr)
				{
					no = no->getEsquerda();
					goto inicio;
				}

				direita:
				if (no->getDireita() != nullptr)
				{
					no = no->getDireita();
					goto inicio;
				}
			}
			else
				return false;
		}

		// verifica se o n� espec�fico est� balanceado
		bool verificaBalanceamentoSingle(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos balanceado o n�
			if (no == nullptr)
				return true;

			// se for folha, consideramos balanceado o n�
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o n� est� balanceado, separadamente dos seus filhos
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return (abs(countD - countE)) < 2;
		}

		// retorna se a direita do n� � maior que a esquerda
		bool verificaDireitaMaior(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos que direita � maior(tanto faz)
			if (no == nullptr)
				return true;

			// se for folha, consideramos que direita � maior(tanto faz)
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o n� tem a direita maior
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return countD > countE;
		}

		// obt�m o valor de balanceamento do n�
		int verificaValor(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return 0;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return 0;

			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return abs(countD - countE);
		}

		// obt�m o valor de balanceamento do n�
		int getNivelMaiorDosMenores(NoArvore<Tipo>* no)
		{
			if (no == nullptr || no->getEsquerda() != nullptr)
				return 0;

			no = no->getEsquerda();
			int nivel;
			for (nivel = 1; no->getDireita() != nullptr; nivel++)
			{
				no = no->getDireita();
			}
			return nivel;
		}

		int getNivelMenorDosMaiores(NoArvore<Tipo>* no)
		{
			if (no == nullptr || no->getDireita() != nullptr)
				return 0;

			no = no->getDireita();
			int nivel;
			for (nivel = 1; no->getEsquerda() != nullptr; nivel++)
			{
				no = no->getEsquerda();
			}
			return nivel;
		}

		// acha o n� errado na �rvore que est� mais para baixo (maior altura)
		NoArvore<Tipo>* acharNoErroneo(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos n�o h� n� errado
			if (no == nullptr)
				return nullptr;
			structAcharNoErroneo* aStruct = new structAcharNoErroneo();
			NoArvore<Tipo>* achado = nullptr;
			
			inicio:
			if (no == nullptr)
			{
				aStruct = dynamic_cast<structAcharNoErroneo*>(pilha.desempilhar());
				achado = aStruct->achado;
				no = aStruct->no;
				if (aStruct->indicacao == 1)
					goto depoisNulo;
				else if (aStruct->indicacao == 2)
					goto depoisDireita;
				else if (aStruct->indicacao == 3)
					goto depoisEsquerda;
				else
					goto depoisTudo;
			}

		depoisNulo:
			//achado = acharNoErroneo(no->getDireita());
			aStruct->no = no;
			aStruct->achado = achado;
			aStruct->indicacao = 2;
			pilha.empilhar(aStruct);
			no = no->getDireita();
			goto inicio;
		depoisDireita:
			achado = aStruct->achado;
			if (achado == nullptr)
			{
				// se n�o achamos, procuramos o n� que est� errado para a esquerda
				//achado = acharNoErroneo(no->getEsquerda());
				aStruct->no = no;
				aStruct->achado = achado;
				aStruct->indicacao = 3;
				pilha.empilhar(aStruct);
				no = no->getDireita();
				goto inicio;
			depoisEsquerda:
				if (achado != nullptr)
				{
					// se achamos, retornamos ele
					while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
						pilha.desempilhar();
					return achado;
				}
			}
			else
			{
				// se achamos, retornamos ele
				while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
					pilha.desempilhar();
				return achado;
			}
			depoisTudo:
			// se o n� atual est� desbalanceado, retornamos ele
			if (!verificaBalanceamentoSingle(no))
			{
				while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
					pilha.desempilhar();
				return no;
			}
			//retornamos nulo se nada est� balanceado
			if (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
			{
				aStruct = dynamic_cast<structAcharNoErroneo*>(pilha.desempilhar());
				achado = aStruct->achado;
				no = aStruct->no;
				if (aStruct->indicacao == 1)
					goto depoisNulo;
				else if (aStruct->indicacao == 2)
					goto depoisDireita;
				else if (aStruct->indicacao == 3)
					goto depoisEsquerda;
				else
					goto depoisTudo;
			}
			else
				return nullptr;
		}

		// gira a �rvore para a dire��o necessitada
		void girar(NoArvore<Tipo>* no, int direcao)
		{
			// se o n� for nulo, n�o giramos
			if (no == nullptr)
				return;
			// se o giro for para a direita, giramos para a direita
			if (direcao == GIRO_PARA_DIREITA)
			{
				// armazenamos os auxiliares
				NoArvore<Tipo>* noFE = no->getEsquerda();
				NoArvore<Tipo>* noFFD = noFE->getDireita();

				// m�todo identifica o modo de como ser� setado a esquerda do n�
				int metodo = 1;
				if (noFFD == nullptr)
				{
					noFFD = noFE->getEsquerda();
					metodo = 2;
				}

				// alteramos de posi��o o n� e o filho dele
				noFE->setDireita(no);
				noFE->setPai(no->getPai());
				no->setPai(noFE);
				if (metodo == 2)
					no->setEsquerda(nullptr);
				else
				{
					no->setEsquerda(noFFD);
					noFFD->setPai(no);
				}

				// setamos a raiz se necess�rio
				if (noFE->getPai() == nullptr)
					this->raiz = noFE;
				else
					noFE->getPai()->setEsquerda(noFE);
			}
			else
			{
				NoArvore<Tipo>* noFE = no->getDireita();
				NoArvore<Tipo>* noFFD = noFE->getEsquerda();

				// m�todo identifica o modo de como ser� setado a direita do n�
				int metodo = 1;
				if (noFFD == nullptr)
				{
					noFFD = noFE->getDireita();
					metodo = 2;
				}

				if (metodo == 2)
				{
					// alteramos de posi��o o n� e o filho dele
					noFE->setEsquerda(no);
					noFE->setPai(no->getPai());
					no->setPai(noFE);
					no->setDireita(nullptr);
					// setamos a raiz se necess�rio
					if (noFE->getPai() == nullptr)
						this->raiz = noFE;
					else
						noFE->getPai()->setDireita(noFE);
				}
				else
				{
					// alteramos de posi��o o n� e o filho dele
					noFE->setEsquerda(nullptr);
					noFFD->setPai(no->getPai());
					no->setPai(noFFD);
					no->setDireita(noFFD->getEsquerda());
					if (no->getDireita() != nullptr)
						no->getDireita()->setPai(no);
					noFE->setPai(noFFD);
					noFFD->setEsquerda(no);
					noFFD->setDireita(noFE);
					// setamos a raiz se necess�rio
					if (noFFD->getPai() == nullptr)
						this->raiz = noFFD;
					else
						noFFD->getPai()->setDireita(noFFD);
				}
			}
		}

		void balancear(int tipo, int dado) // 0 - inclus�o, 1- exclus�o
		{
			// verificar se o balanceamento est� correto
			// se sim, acabou...
			if (!verificaBalanceamento(this->raiz))
			{
				// se n�o, temos que ajustar
				cout << "BOT> Desbalanceou a arvore" << endl;
				// se � exclus�o
				if (tipo == 1)
				{
					// achamos o n� errado e o seu pai
					NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);
					NoArvore<Tipo>* pai = oi;
					do
					{
						// setamos o pai
						pai = pai->getPai();

						// giramos para o lado certo
						if (verificaDireitaMaior(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);

						// colocamos o atual como pai
						oi = pai;
						// enquanto n�o est� com o pai nulo (raiz)
					} while (pai != nullptr);
				}
				// se � inclus�o
				else
				{
					do
					{
						// achamos o n� errado
						NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);

						// giramos para o lado certo
						if (verificaDireitaMaior(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);
					// enquanto n�o est� balanceado
					} while (!(verificaBalanceamento(this->raiz)));
				}
				cout << "BOT> Tentativa de balanceamento realizada" << endl;
			}
			else
			{
				cout << "BOT> Nao desbalanceou a arvore" << endl;
			}
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// raiz da �rvore inteira
		NoArvore<Tipo> *raiz;
		Pilha<IEmpilhavel*> pilha;
	private:
		static bool comparaParaEsvaziar(int a, int b)
		{
			return (a == b);
		}
};