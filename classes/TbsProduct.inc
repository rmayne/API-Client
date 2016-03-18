<?php

$dir = dirname(__FILE__);
require_once('TbsApi.inc');
require_once('TbsObject.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/15/2016
* 
*toodo: make a skip in get collection; the data in the first couple 100 is making update fail!
*/

class TbsProduct extends TbsObject
{
	private $TbsApi;

	function __construct(){
		// maybe this should use dependancy injection?
		$this->TbsApi = TbsApi::getInstance();
	}

	/*
	* retrieve a collection of product objects
	*
	* Params
	* $howMany : the number of records to return
	* 
	* Returns : an array of object upon success, false upon failure
	*
	* TODO: the objects returned should be Tbs product not generic
	*/
	function getCollection($howMany){
		return $this->TbsApi->getResource('products', null, $howMany);
	}

	/*
	* retrieve a specific product object
	*
	* Params
	* $productsKey : the unique key for the product
	*
	* returns : $this, with the retrieved product data applied, false upon failure
	*/
	function get($productsKey){
		$result =  $this->TbsApi->getResource('products', $productsKey);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	/*
	* get the default values for a given product form
	* Params: 
	* $ProductFormsKey : int, the product form key
	*
	* return : $this upon succes, false upon failure
	*/
	function getDefaults($ProductFormsKey){
		// retrieve product defaults
		$result = $this->TbsApi->getResource("ProductForms/$ProductFormsKey/productdefaults", false, false);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	function getProductForms(){
		//$productFormList = $this->getresource('ProductForms', false);
	}


	/*
	* creates a product
	*
	* return: object if successful, false upon failure
	* 
	*/
    function create(){
		$result = $this->TbsApi->post('products', $this);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
    }


	/*
	* updates a product record with the data set on this object
	*
	* return : object with data returned from remote server, false upon failure
	*
	* notes : BE AWARE THAT THERE IS BAD DATA IN THE DB. REPOSTING IT WILL CAUSE A MYSTERIOUS 500 ERROR
	*  also, read the notes on changing customers keys 
	* and child products at : https://demo3.precisebillonline.com/OSNDemo/RestService/TBSRestService/help
	*/ 
	function update(){
		$result = $this->TbsApi->put('products' . '/' . $this->ProductsKey, $this);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	/*
	*  check for a duplicate product
	* Params:
	* $productId : the the ProductID field will be searched for this inorder to find a match
	* returns : an array of duplicate objects, false upon failure
	*
	* TODO : product Id should not be passed, it should be applied to this object before calling
	*/
	function checkForDuplicate($productId){
		$filters = array(
			array(
				"field" => 'ProductID',
				"filter" => 'Equal',
				"value" => $productId
			)
		);

		$result = $this->TbsApi->getResource('products', null, 'All', $filters, null);

		if(gettype($result[0]) == 'object'){
			return $result;
		} else {
			return false;
		}
	}


}

?>