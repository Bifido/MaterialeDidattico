using UnityEngine;

public interface PassiveBackgroundGen{

	// Use this for initialization
	void Init(int capacity,GameObject prefab,float xPos,float yOffset,float yLimit);

	void StartMove();

	void Update();

	void StopMove();
	
	void ResetMove();
}
