using UnityEngine;
using System.Collections;

public class BackGroundMovement : MonoBehaviour,Executable,Allocable {

	//EXECUTABLE INTERFACE-------------------
	[SerializeField] private Executioner 	m_oExecutioner;
	[SerializeField] private Allocator		m_oAllocatorOwner;
	
	public void Execute(){
		this.enabled = true;
	}
	public void StopExecute(){
		this.enabled = false;
	}
	public bool IsExecuting(){
		return this.enabled;
	}	
	public void SetExcutioner(Executioner exec){
		this.m_oExecutioner = exec;
	}
	public void NotifyExecutioner(bool success){

	}	
	public void SetAllocator(Allocator alloc){
		this.m_oAllocatorOwner = alloc;
	}
	public Allocator GetAllocator(){
		return this.m_oAllocatorOwner;
	}

	public GameObject GetGameObject(){
		return this.gameObject;
	}
	//EXECUTABLE INTERFACE-------------------

	public void SetYLimit(float yLimit){
		this.m_fYLimit = yLimit;
	}

	public void SetSpawnPos(Vector3 spawnPos){
		this.m_vSpawnPos = spawnPos;
	}

	// Use this for initialization
	void Start () {
		this.StopExecute();
		this.m_fSpeed = Constants.BACKGROUND_SPEED;
	}
	
	// Update is called once per frame
	void Update () {
		if(this.transform.position.y <= m_fYLimit){
			this.transform.position = m_vSpawnPos;
		}else{
			this.transform.Translate(Vector3.right*Time.deltaTime*this.m_fSpeed);
		}
	}

	private float 	m_fSpeed;
	private float 	m_fYLimit;
	private Vector3	m_vSpawnPos;
}
