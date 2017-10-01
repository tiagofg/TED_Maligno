#pragma once
using namespace std;
#include <string>

#define TAMANHO_PILHA 100
typedef bool(*fn)(int a, int b);

template <class Tipo>
class Pilha
{
public:
	// ------------------------------------------------------------------------------------------------------------------------------------------------------ -//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	Pilha(const unsigned int novoTamanhoMax) : tamanhoMax(novoTamanhoMax), topo(-1)
	{
		this->nos = (Tipo*)malloc(novoTamanhoMax*sizeof(Tipo));
	}

	Pilha(const Pilha<Tipo> &original) : tamanhoMax(original.tamanhoMax), topo(original.topo)
	{
		this->nos = (Tipo*)malloc(original.tamanhoMax*sizeof(Tipo));
		for (int i = 0; i <= original.topo; i++)
			this->nos[i] = novoDado;
	}

	Pilha() : tamanhoMax(TAMANHO_PILHA), topo(-1)
	{
		this->nos = (Tipo*)malloc(TAMANHO_PILHA*sizeof(Tipo));
	}

	~Pilha()
	{
		//free(this->nos);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------CONSTRUTORES DE C�PIA--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	Pilha<Tipo>* clone() const
	{
		return new Pilha<Tipo>(*this);
	}

	void operator= (Pilha<Tipo> &outro)
	{
		Pilha<Tipo> aux;
		for (int i = outro.length()-1; i > -1; i--)
		{
			aux.empilhar(outro.desempilhar());
			this->nos[i] = (aux.getTopo());
		}
		while (!aux.ehVazia())
			outro.empilhar(aux.desempilhar());
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------GETTERS E SETTERS----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	int length() const
	{
		return this->tamanhoMax;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------METODOS AUXILIARES----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	bool ehCheia() const
	{
		return this->topo >= this->tamanhoMax;
	}

	bool ehVazia() const
	{
		return this->topo < 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------M�TODOS PRINCIPAIS---------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	bool empilhar(const Tipo& novoDado)
	{
		if (this->ehCheia())
			return false;
		this->nos[++this->topo] = novoDado->clone();
		return true;
	}

	Tipo desempilhar()
	{
		if (!this->ehVazia())
			return (this->nos[this->topo--]);
		return NULL;
	}

	void esvaziar()
	{
		while (!this->ehVazia())
			this->desempilhar();
	}

	void esvaziar(fn funcao, const Tipo &tipo)
	{
		while (!this->ehVazia())
		{
			if ((*funcao)(tipo, this->topo))
				this->desempilhar();
			else
				return;
		}
	}

	Tipo getTopo() const
	{
		if (!this->ehVazia())
			return this->nos[this->topo];
		else
			return NULL;
	}

	friend ostream& operator<< (ostream &os, const Pilha<Tipo> &aPilha)
	{
		return (os << aPilha.toString());
	}

	friend class Arvore<Tipo>;
protected:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------METODO APOCALIPTICO-----------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	string toString() const
	{
		string txt = "[ ";
		for (int i = 0; i <= this->topo; i++)
			txt += to_string(this->nos[i]) + "; ";
		return txt + "]";
	}
private:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	Tipo *nos;
	int tamanhoMax, topo;
};