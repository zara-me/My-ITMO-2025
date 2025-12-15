namespace AutoBattler
{
    public class Warrior : Hero
    {
        public Warrior(string name, int hp, int defense, int damage)
            : base(name, hp, defense, damage) { }

        public override void SpecialAbility(Hero target)
        {
            if (target == null) return;
            target.TakeDamage(Damage * 2);
        }
    }
}
