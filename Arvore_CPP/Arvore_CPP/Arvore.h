#pragma once
#include "NoArvore.h"
#include "stdio.h"
#include <iostream>
#include <string>
using namespace std;
#define GIRO_PARA_ESQUERDA 0
#define GIRO_PARA_DIREITA 1

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
		//--------------------------------------------------------------------M�TODOS----------------------------------------------------------------------------//
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
				{
					this->raiz = nullptr;
				}
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
				this->balancear(1);
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
			this->incluir(novoDado, raiz);
			// balanceamos
			this->balancear(0);
		}

		friend ostream& operator<< (ostream &os, const Arvore<Tipo> &aArvore)
		{
			// escrevemos o valor da �rvore
			return (os << "TREE> " << aArvore.toString());
		}

		string toString() const
		{
			// retornamos o conte�do da �rvore desde a raiz
			return this->toString(this->raiz);
		}

	protected:
		string toString(NoArvore<Tipo> *no) const 
		{
			// m�todo recursivo que coloca os da esquerda e os da direita e o pr�prio
			if (no == nullptr)
				return "";
			return ((no->getEsquerda()==nullptr)?"": ("("+this->toString(no->getEsquerda()) + ")<-")) + "(" + to_string(no->getDado()) + ")" + ((no->getDireita() == nullptr) ? "" : ("->("+this->toString(no->getDireita())+")"));
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

		// incl�i recursivamente o dado entregue embaixo do n� dado, se poss�vel
		void incluir(const Tipo &novoDado, NoArvore<Tipo> *no)
		{
			if (novoDado > no->getDado())
			{
				if (no->getDireita() != nullptr)
					incluir(novoDado, no->getDireita());
				else // if(direita � null)
				{
					no->setDireita(new NoArvore<Tipo>(novoDado));
					no->getDireita()->setPai(no);
				}
			}
			else if (novoDado < no->getDado())
			{
				if (no->getEsquerda() != nullptr)
					incluir(novoDado, no->getEsquerda());
				else // if(esquerda � null)
				{
					no->setEsquerda(new NoArvore<Tipo>(novoDado));
					no->getEsquerda()->setPai(no);
				}
			}
			// se for igual, n�o incl�i
			return;
		}

		// incl�i uma sub�rvore inteira embaixo do n�
		void incluirSubArvore(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
				return;

			incluir(no->getDado());
			incluirSubArvore(no->getDireita());
			incluirSubArvore(no->getEsquerda());
		}

		// incl�i uma sub�rvore inteira embaixo do n�
		void incluirSubArvore(NoArvore<Tipo> *no, NoArvore<Tipo> *ondeIncluir)
		{
			if (no == nullptr)
				return;

			incluir(no->getDado(), ondeIncluir);
			incluirSubArvore(no->getDireita(), ondeIncluir);
			incluirSubArvore(no->getEsquerda(), ondeIncluir);
		}

		// excl�i (deleta) uma sub�rvore inteira (n�o precisa estar na �rvore)
		void excluirSubArvore(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
				return;

			excluirSubArvore(no->getDireita());
			excluirSubArvore(no->getEsquerda());
			no = nullptr;
		}

		// excl�i recursivamente o n� com o dado desejado
		bool excluirRecur(const Tipo &dado, NoArvore<Tipo> *atual)
		{
			// n� inexistente
			if (atual == nullptr)
				return false;

			if (dado == atual->getDado())
			{
				if (atual->isFolha()) // se � folha, s� excluir normal
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
					{
						atual->getPai()->setDireita(nullptr);
					}
					else
					{
						atual->getPai()->setEsquerda(nullptr);
					}
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
					if (verificaDireitaMaior(atual))
					{
						if (atual->getDirecaoPai() == PAI_ESQUERDA)
							atual->getPai()->setDireita(atual->getDireita());
						else
							atual->getPai()->setEsquerda(atual->getDireita());
						atual->getDireita()->setPai(atual->getPai());

						// inclui a sub�rvore da direita na raiz
						incluirSubArvore(atual->getEsquerda(), atual->getPai());
						// exclui a sub�rvore da direita
						excluirSubArvore(atual->getEsquerda());
					}
					else
					{
						if (atual->getDirecaoPai() == PAI_ESQUERDA)
							atual->getPai()->setDireita(atual->getEsquerda());
						else
							atual->getPai()->setEsquerda(atual->getEsquerda());
						atual->getEsquerda()->setPai(atual->getPai());
						

						this->balancear(1);
						// inclui a sub�rvore da direita no pai
						incluirSubArvore(atual->getDireita(), this->raiz);
						// exclui a sub�rvore da direita
						excluirSubArvore(atual->getDireita());
					}

					atual = nullptr;
				}
				return true;
			}
			else if (dado < atual->getDado())
				return this->excluirRecur(dado, atual->getEsquerda());
			else
				return this->excluirRecur(dado, atual->getDireita());
		}

		// m�todo recursivo que coloca no "maior" o valor do maior n� errado
		void maiorNivelRecur(NoArvore<Tipo>* no, int aux, int* maior)
		{
			if (no == nullptr)
				return;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				// coloca no maior se o n�vel da folha for maior que o maior antigo
				if (aux > *maior)
					*maior = aux;
				return;
			}

			maiorNivelRecur(no->getDireita(), aux+1, maior);
			maiorNivelRecur(no->getEsquerda(), aux+1, maior);
		}

		// retorna a maior altura de um n�
		int maiorNivel(NoArvore<Tipo>* no)
		{
			int* maior = (int*)malloc(sizeof(int));
			*maior = 0;
			maiorNivelRecur(no, 1, maior);
			return *maior;
		}

		// https://www.youtube.com/watch?v=zh27Tp8HV7E
		// verifica se a sub�rvore inteira est� balanceada
		bool verificaBalanceamento(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos balanceado o n�
			if (no == nullptr)
				return true;

			// se for folha, consideramos balanceado o n�
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o n� est� balanceado, juntamente com os seus filhos
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return (abs(countD-countE)) < 2 && verificaBalanceamento(no->getEsquerda()) && verificaBalanceamento(no->getDireita());
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
				return true;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return abs(countD - countE);
		}

		// acha o n� errado na �rvore que est� mais para baixo (maior altura)
		NoArvore<Tipo>* acharNoErroneo(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos n�o h� n� errado
			if (no == nullptr)
				return nullptr;

			// procuramos o n� que est� errado para a direita
			NoArvore<Tipo>* achado = acharNoErroneo(no->getDireita());
			if (achado == nullptr)
			{
				// se n�o achamos, procuramos o n� que est� errado para a esquerda
				achado = acharNoErroneo(no->getEsquerda());
				if (achado != nullptr)
					// se achamos, retornamos ele
					return achado;
			}
			else
				// se achamos, retornamos ele
				return achado;

			// se o n� atual est� desbalanceado, retornamos ele
			if (!verificaBalanceamentoSingle(no))
				return no;
			//retornamos nulo se nada est� balanceado
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

		void balancear(int tipo) // 0 - inclus�o, 1- exclus�o
		{
			// verificar se o balanceamento est� correto
			// se sim, acabou...
			if (!verificaBalanceamento(this->raiz))
			{
				// se n�o, temos que ajustar
				cout << "BOT> Desbalanceou a arvore na raiz " << this->raiz->getDado() << endl;
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
				cout << "BOT> Tentativa de balanceamento realizada com nova raiz " << this->raiz->getDado() << endl;
				cout << *this << endl;
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
	private:
};