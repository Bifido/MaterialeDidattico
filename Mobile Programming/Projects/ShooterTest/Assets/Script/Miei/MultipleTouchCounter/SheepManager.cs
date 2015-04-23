using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class SheepManager : MonoBehaviour {

	// Use this for initialization
	void Start () {
		this.m_lSheepList = new ArrayList(m_sSheepCount);
		this.m_aSheepAllocator = new Allocator(m_sSheepCount,m_oSheepPrefab);
		for(int i = 0; i< m_sSheepCount; i++){
			m_lSheepList.Add(m_aSheepAllocator.GetElement());
		}
	}
	
	// Update is called once per frame
	void Update () {
	}

	private void OnEnable(){
		if(MyInputManager == null){
			Debug.LogError("No InputManager in the scene");
			return;
		}		
		MyInputManager.OnNewTouch += UpdateText;
		MyInputManager.OnTouchChangedEvent += OnTouchChanged;
		MyInputManager.OnEndTouch += OnTouchDismiss;
	}

	//update text counter
	private void UpdateText(short touchCount) {
		this.TextCounter.UpdateText(touchCount);
	}

	private void OnTouchChanged(short id,Vector3 pos){
		GameObject sheep = m_lSheepList[id] as GameObject;
		sheep.transform.position = pos;
	}

	private void OnTouchDismiss(short id){
		Debug.Log ("Dismiss "+id);
		GameObject sheep = m_lSheepList[id] as GameObject;
		sheep.transform.position = new Vector3(2f,2f,0);
	}

	public TextCounter		TextCounter;
	public Text				TextObj;
	public MyInputManager 	MyInputManager;
	public GameObject 		m_oSheepPrefab;

	private const short		m_sSheepCount = 10;
	private Allocator 		m_aSheepAllocator;
	private ArrayList 		m_lSheepList;
}
