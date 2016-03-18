<?php

class TbsObject {
	/*
	* Author: Reiser Mayne , github.com/rmayne
	* Date: 1/15/2016
	* 
	* merges a data object with $this object, identically 
	* named properties will be overwritten by the new object
	* Params 
	*  $data : the object containing the data to be merged into $this object
	* returns : $this
	*/
	
	function applyData($data){
		$object = $this;
		$data = get_object_vars($data);
		foreach ($data as $key => $value) {
			if(is_object($value)){
				if(!property_exists($object, $key)){
					$newObject = new stdClass();
					$object->$key = $newObject;
				}
				$this->applyData($object->$key, $value);
			} else {
				$object->$key = $value;
			}
		}
		return $this;
	}
}

?>