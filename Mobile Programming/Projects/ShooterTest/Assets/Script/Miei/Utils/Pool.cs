using UnityEngine;
using System.Runtime.CompilerServices;
using System.Collections;
using System.Collections.Generic;

public class Pool<U>{

	public Pool(){
		this.m_oElements = null;
		this.isFree = null;
	}

	public Pool(int capacity, U [] elements){
		//init array and free elements list
		this.m_oElements = elements;
		this.isFree = new bool[elements.Length];

		//populate the index list
		for(int i = 0;i<this.m_oElements.Length;i++){
			this.isFree[i] = true;
		}
	}

	[MethodImpl(MethodImplOptions.Synchronized)]
	public U getElement(){
		U toReturn = default(U);
		for(int i = 0;i<this.m_oElements.Length;i++){
			if(this.isFree[i]){
				this.isFree[i] = false;
				toReturn = this.m_oElements[i];
				break;
			}
		}
		return toReturn;
	}

	[MethodImpl(MethodImplOptions.Synchronized)]
	public void freeElement(U element){
		for(int i = 0; i<this.m_oElements.Length; i++){
			if(element.Equals(this.m_oElements[i])){
				this.isFree[i] = true;
			}
		}
	}

	private U [] m_oElements;
	private bool [] isFree;
}
