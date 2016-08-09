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
		$properties = get_object_vars($data);
		foreach ($properties as $key => $value) {
			if(property_exists($this, $key) && is_object($value)){
				$properties2 = get_object_vars($value);
				foreach ($properties2 as $key2 => $value2) {
					$object->$key->$key2 = $value2;
				}
			} else {
				$this->$key = $value;
			}
		}
		return $this;
	}
}

?>