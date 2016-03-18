<?php

$dir = dirname(__FILE__);
require_once('TbsApi.inc');
require_once('TbsObject.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/15/2016
* 
*/

class TbsDetail extends TbsObject
{
	private $TbsApi;

	function __construct(){
		// maybe this should use dependancy injection?
		$this->TbsApi = TbsApi::getInstance();
	}

	/*
	* retrieve a collection of detail objects
	*
	* Params
	* $howMany : the number of records to return
	* 
	* Returns : an array of object upon success, false upon failure
	*
	* TODO: the objects returned should be Tbs detail not generic
	*/
	function getCollection($howMany){
		return $this->TbsApi->getResource('details', null, $howMany);
	}

	/*
	* retrieve a specific detail object
	*
	* Params
	* $detailsKey : the unique key for the detail
	*
	* returns : $this, with the retrieved detail data applied, false upon failure
	*/
	function get($detailsKey){
		$result =  $this->TbsApi->getResource('details', $detailsKey);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	/*
	* get the default values for a given detail form
	* Params: 
	* 
	*
	* return : $this upon succes, false upon failure
	*/
	function getDefaults(){
		// retrieve detail defaults
		$result = $this->TbsApi->getResource("details/defaults", false, false);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	/*
	* creates a detail
	*
	* return: object if successful, false upon failure
	* 
	*/
    function create(){
		$result = $this->TbsApi->post('details', $this);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
    }

	/*
	* updates a detail record with the data set on this object
	*
	* return : object with data returned from remote server, false upon failure
	*
	* notes : BE AWARE THAT THERE IS BAD DATA IN THE DB. REPOSTING IT WILL CAUSE A MYSTERIOUS 500 ERROR
	*/ 
	function update(){
		$result = $this->TbsApi->put('details' . '/' . $this->DetailsKey, $this);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}
}