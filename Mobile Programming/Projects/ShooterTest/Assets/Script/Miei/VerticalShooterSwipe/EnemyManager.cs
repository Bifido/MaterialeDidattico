using UnityEngine;
using System.Collections;

public class EnemyManager : MonoBehaviour,Executioner {

	//EXECUTIONER INTERFACE---------------------------------------------------------------
	public void NotifyExecuteEndFail(Executable executable){
		if(this.m_fTimeGeneration > 1f){
			this.m_fTimeGeneration -= 0.1f;
		}
		this.m_oEnemies.FreeElement(executable as EnemyMovement);
	}
	public void NotifyExecuteEndSuccess(Executable executable){
		//Should reset
	}
	//EXECUTIONER INTERFACE---------------------------------------------------------------


	// Use this for initialization
	void Start () {
		this.m_fTimeGeneration = 2f;
		this.m_bCanGenerate = true;

		//enemies allocator
		this.m_oEnemies = new AllocatorMonoBehaviour<EnemyMovement>(20,enemyPrefab);

		this.StartCoroutine(GenerateEnemies());
	}

	IEnumerator GenerateEnemies(){
		while(this.m_bCanGenerate){
			yield return new WaitForSeconds(this.m_fTimeGeneration);
			EnemyMovement enemy = this.m_oEnemies.GetElement();
			if(enemy != null){
				enemy.SetExcutioner(this);
				enemy.transform.position = new Vector3(Random.Range(-2.3f,2.3f),6.3f,0f);
				enemy.collider.isTrigger = false;
				enemy.Execute();
			}else{
				Debug.Log("Ops... no enemy!");
			}
		}
	}

	public GameObject	 									enemyPrefab;
	private AllocatorMonoBehaviour<EnemyMovement>			m_oEnemies;
	[SerializeField] private float							m_fTimeGeneration;
	[SerializeField] private bool							m_bCanGenerate;
}
