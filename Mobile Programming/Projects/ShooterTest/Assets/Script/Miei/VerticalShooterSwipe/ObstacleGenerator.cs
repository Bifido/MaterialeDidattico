using UnityEngine;
using System.Collections;

public class ObstacleGenerator : MonoBehaviour,Executioner {

	// Use this for initialization
	void Start () {
		this.m_fTimeGeneration = Constants.OBSTACLE_TIME_GEN;
		
		//enemies allocator
		this.m_oEnemies = new AllocatorMonoBehaviour<EnemyMovement>(Constants.OBSTACLE_ENEMY_POOL_ELEMENTS,enemyPrefab);
		
		//aggiungo il timer
		this.m_oTimer = gameObject.AddComponent<Timer>();

		//Init Generation
		this.Init();
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void Init(){
		//timer set up.
		this.m_oTimer.Start(m_fTimeGeneration,GenerateObstacle);
	}

	public void GenerateObstacle(){
		EnemyMovement enemy = this.m_oEnemies.GetElement();
		if(enemy != null){
			enemy.SetExcutioner(this,(int)OBSTACLE_ID.ENEMY);
			enemy.SetAllocator(this.m_oEnemies);
			enemy.transform.position = new Vector3(Constants.COLUMN_POS_A[Random.Range(0,(int)Constants.COLUMN_POS_X.COUNT)],
			                                       Constants.OBSTACLE_Y_OFFSET,
			                                       0f);
			enemy.GetComponent<Collider>().isTrigger = false;
			enemy.Execute();
		}else{
			Debug.Log("Ops... no enemy!");
		}
		this.m_oTimer.Start(m_fTimeGeneration,GenerateObstacle);
	}

	public void StopGenerate(){
		this.m_oTimer.FinishTimer();
	}

	public void Reset(){

	}

	//EXECUTIONER INTERFACE---------------------------------------------------------------
	public void NotifyExecuteEndFail(Executable executable){
		int exec_ID = executable.GetID();
		switch(exec_ID){
		case 0:
			this.m_oEnemies.FreeElement(executable as EnemyMovement);
			break;
		default:
			Debug.Log("NotifyExecuteEndFail in ObstacleGenerator failed with ID:"+exec_ID);
			break;
		}
	}
	public void NotifyExecuteEndSuccess(Executable executable){
		int exec_ID = executable.GetID();
		switch(exec_ID){
		case 0:
			this.m_oEnemies.FreeElement(executable as EnemyMovement);
			Debug.Log("Player Touched!");
			break;
		default:
			Debug.Log("NotifyExecuteEndFail in ObstacleGenerator failed with ID:"+exec_ID);
			break;
		}
	}
	//EXECUTIONER INTERFACE---------------------------------------------------------------

	public enum OBSTACLE_ID { ENEMY = 0, COUNT };

	public GameObject	 									enemyPrefab;
	private AllocatorMonoBehaviour<EnemyMovement>			m_oEnemies;
	private float											m_fTimeGeneration;
	private Timer 											m_oTimer;
}
