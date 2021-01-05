
node* inserisciInCoda(node* n, message* m){
  node *prec;
  node *tmp;

  tmp = (node*) malloc(sizeof(node));
  if(tmp != NULL){
    tmp->next = NULL;
    tmp->message = m;
    if(lista == NULL)
      lista = tmp;
    else{
      /*raggiungi il termine della lista*/
      for(prec=lista;prec->next!=NULL;prec=prec->next);
      prec->next = tmp;
    }
  } else
      printf("Memoria esaurita!\n");
  return n;
}

node* inserisciInTesta(node* n, message* m){
  node *tmp;

  tmp = (node*) malloc(sizeof(node));
  if(tmp != NULL){
    tmp->message = m;
    tmp->next = n;
    n = tmp;    
  } else
      printf("Memoria esaurita!\n");
  return n;
}

node* getNext(node* n){
  return n->next;
}
int hasNext(node* n ){
  if(n->next == NULL){
    return 0;
  }
  return 1
}