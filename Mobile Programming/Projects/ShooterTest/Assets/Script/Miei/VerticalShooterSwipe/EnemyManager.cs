using UnityEngine;
using System.Collections;

public class EnemyManager : MonoBehaviour,Executioner {

	//EXECUTIONER INTERFACE---------------------------------------------------------------
	public void NotifyExecuteEndFail(Executable executable){
//		if(this.m_fTimeGeneration > 1f){
//			this.m_fTimeGeneration -= 0.1f;
//		}
		this.m_oEnemies.FreeElement(executable as EnemyMovement);
	}
	public void NotifyExecuteEndSuccess(Executable executable){
		//Should reset
	}
	//EXECUTIONER INTERFACE---------------------------------------------------------------


	// Use this for initialization
	void Start () {
		this.m_fTimeGeneration = Constants.ENEMY_SPAWN_TIMER;

		//enemies allocator
		this.m_oEnemies = new AllocatorMonoBehaviour<EnemyMovement>(Constants.ENEMY_POOL_ELEMENTS,enemyPrefab);

		//aggiungo il timer
		this.m_oTimer = gameObject.AddComponent<Timer>();
		//lo registro
		this.m_oTimer.Start(m_fTimeGeneration,GenerateEnemies);
	}

	public void GenerateEnemies(){
		EnemyMovement enemy = this.m_oEnemies.GetElement();
		if(enemy != null){
			enemy.SetExcutioner(this);
			int posIndex = Random.Range(0,(int)Constants.COLUMN_POS_X.COUNT);
			enemy.transform.position = new Vector3(Constants.COLUMN_POS_A[posIndex],6.3f,0f);
			enemy.GetComponent<Collider>().isTrigger = false;
			enemy.Execute();
		}else{
			Debug.Log("Ops... no enemy!");
		}
		this.m_oTimer.Start(m_fTimeGeneration,GenerateEnemies);
	}

	public GameObject	 									enemyPrefab;
	private AllocatorMonoBehaviour<EnemyMovement>			m_oEnemies;
	private float											m_fTimeGeneration;
	[SerializeField] private bool							m_bCanGenerate;
	private Timer 											m_oTimer;
}
