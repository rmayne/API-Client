<?php
require_once('../include/TbsProduct.inc');
require_once('../include/TbsApi.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/27/2016
* TODO: 
*/

abstract class CmdServiceOrderItemMediator
{

	/*
	* NOTES:
	* this only works for the product form/type 5 , VOIP DID
	*
	*/
	function toTbsParentProduct(ServiceOrderItem $serviceOrderItem, TbsProduct $tbsProduct){
		$id = $serviceOrderItem->getId();
		$sql = "SELECT 
				soi.*,
				cba.tbs_account_number,
				cs.tbs_locations_key
			FROM service_order_item soi
			JOIN service_order so
				ON so.id = soi.service_order_id 
			JOIN client_billing_account cba 
				ON cba.id = so.client_ban
			JOIN client_site cs
				ON cs.id = so.a_site_id
			WHERE soi.id = $id
			LIMIT 1";

		// extract fileds from sql result
		$sqlResult = sqlExecute($sql);
		$tbsAccountNumber = $sqlResult->Fields('tbs_account_number');
		$tbsApi = TbsApi::getInstance();
		$tbsCustomersKey = $tbsApi->getCustomersKeyFromAccountNumber($tbsAccountNumber);
		$tbsLocationsKey = $sqlResult->Fields('tbs_locations_key');
		$itemId = $sqlResult->Fields('itemid');
		$status = $sqlResult->Fields('status');

		// set the product properties
		$tbsProduct->CustomersKey = (int) $tbsCustomersKey; //use int
		$tbsProduct->LocationsKey = (int) $tbsLocationsKey; //use int, MUST EXIST DB
		$tbsProduct->ProductID  = rand(1, 1000000000); //$itemId; // must be unique, appears to be a Alpha sku, or description
		$tbsProduct->ProductFormsKey = 2; 
		//this is set by the api i think		$tbsProduct->ProductsKey =  0;
		$tbsProduct->Status = "Pending";//($status) ? $status : "Pending"; //"Pending" definitly works with the API : Active, Disconnected

		return $tbsProduct;
	}
}






